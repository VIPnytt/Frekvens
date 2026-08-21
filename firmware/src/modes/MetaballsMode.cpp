#if MODE_METABALLS

#include "modes/MetaballsMode.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

#include <nvs.h>

static_assert(GRID_COLUMNS * GRID_ROWS >= 50U,
              __STRING(MODE_METABALLS) " is not compatible with this device's display size.");

/**
 * @brief Loads persisted metaballs settings and publishes the active configuration.
 */
void MetaballsMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _speed{0U};
        if (nvs_get_u8(handle, "speed", &_speed) == ESP_OK)
        {
            speedFactor = _speed;
            speed = static_cast<float>(speedFactor) * baseSpeed;
        }
        uint8_t _radius{0U};
        if (nvs_get_u8(handle, "radius", &_radius) == ESP_OK)
        {
            radiusFactor = _radius;
            updateRadius();
        }
        nvs_close(handle);
    }
    transmit();
}

/**
 * @brief Initializes metaballs positions and velocities.
 */
void MetaballsMode::begin()
{
    for (size_t idx{0U}; idx < contributions.size(); ++idx)
    {
        contributions[idx] = ((UINT8_MAX - idx) * (UINT8_MAX - idx) * (0b1U << 6U)) >> (0b1U << 4U);
    }
    for (Ball &ball : balls)
    {
        ball.x = static_cast<float>(random(GRID_COLUMNS));
        ball.y = static_cast<float>(random(GRID_ROWS));
        ball.xVelocity = speed * static_cast<float>(random(1, multiplier) * ((random(2) * 2) - 1));
        ball.yVelocity = speed * static_cast<float>(random(1, multiplier) * ((random(2) * 2) - 1));
    }
    Display.fillFrame(0U);
}

/**
 * @brief Updates the metaballs positions and calculates their contributions to the display.
 *
 * The metaballs are represented as circles that move across the display, and their brightness
 * contributions are calculated based on their distance from each pixel.
 */
void MetaballsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (!Extensions.Microphone().isTriggered())
    {
        return;
    }
#endif // EXTENSION_MICROPHONE
#if PITCH_HORIZONTAL != PITCH_VERTICAL
    const bool rotated{(static_cast<uint8_t>(Display.getOrientation()) & 0b1U) != 0U};
    const float xRatio{static_cast<float>(2U * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL)) /
                       static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL)};
    const float yRatio{static_cast<float>(2U * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL)) /
                       static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL)};
#endif // PITCH_HORIZONTAL != PITCH_VERTICAL
    for (const Ball &ball : balls)
    {
        const uint8_t yMax{static_cast<uint8_t>(
            min<int8_t>(static_cast<int8_t>(ceilf(ball.y + radius - min(ball.yVelocity, .0F))), GRID_ROWS - 1U))};
        const uint8_t yMin{
            static_cast<uint8_t>(max<int8_t>(static_cast<int8_t>(ball.y - radius - max(ball.yVelocity, .0F)), 0))};
        for (uint8_t x{
                 static_cast<uint8_t>(max<int8_t>(static_cast<int8_t>(ball.x - radius - max(ball.xVelocity, .0F)), 0))};
             x <= static_cast<uint8_t>(min<int8_t>(
                      static_cast<int8_t>(ceilf(ball.x + radius - min(ball.xVelocity, .0F))), GRID_COLUMNS - 1U));
             ++x)
        {
            for (uint8_t y{yMin}; y <= yMax; ++y)
            {
                uint8_t brightness{0U};
                for (const Ball &ball : balls)
                {
#if PITCH_HORIZONTAL == PITCH_VERTICAL
                    const float xDistance{ball.x - static_cast<float>(x)};
                    const float yDistance{ball.y - static_cast<float>(y)};
#else
                    const float xDistance{(ball.x - static_cast<float>(x)) * xRatio};
                    const float yDistance{(ball.y - static_cast<float>(y)) * yRatio};
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
                    const float distanceSq{(xDistance * xDistance) + (yDistance * yDistance)};
                    if (distanceSq < radiusSq)
                    {
                        brightness = static_cast<uint8_t>(min<uint16_t>(
                            static_cast<uint16_t>(brightness) + contributions[static_cast<uint8_t>(min<float>(
                                                                    distanceSq * feathering / radiusSq, 255.0F))],
                            UINT8_MAX));
                        if (brightness == UINT8_MAX)
                        {
                            break;
                        }
                    }
                }
                Display.setPixel(x, y, brightness);
            }
        }
    }
    for (Ball &ball : balls)
    {
        ball.x += ball.xVelocity;
        ball.y += ball.yVelocity;
        if (ball.x < .0F)
        {
            ball.x = .0F;
            ball.xVelocity = speed * static_cast<float>(random(1, multiplier));
        }
        else if (ball.x > GRID_COLUMNS - 1U)
        {
            ball.x = GRID_COLUMNS - 1U;
            ball.xVelocity = -speed * static_cast<float>(random(1, multiplier));
        }
        if (ball.y < .0F)
        {
            ball.y = .0F;
            ball.yVelocity = speed * static_cast<float>(random(1, multiplier));
        }
        else if (ball.y > GRID_ROWS - 1U)
        {
            ball.y = GRID_ROWS - 1U;
            ball.yVelocity = -speed * static_cast<float>(random(1, multiplier));
        }
    }
}

/**
 * @brief Sets the base speed of the metaballs and stores it in non-volatile storage.
 *
 * Uses arbitrary units for speed, where 1 is the slowest and 11 is the fastest.
 * The actual speed is calculated based on the base speed and the speed factor.
 *
 * @param _speed New speed factor for the metaballs.
 */
void MetaballsMode::setSpeed(uint8_t _speed)
{
    if (_speed < 1U)
    {
        speedFactor = 1U;
    }
    else if (_speed > 11U)
    {
        speedFactor = 11U;
    }
    else
    {
        speedFactor = _speed;
    }

    speed = baseSpeed * static_cast<float>(speedFactor);

    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "speed", static_cast<uint8_t>(speedFactor));
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
}

/**
 * @brief Sets the radius of the metaballs and stores it in non-volatile storage.
 *
 * Uses arbitrary units for _radius, where 1 is the smallest and 10 is the largest.
 * The actual radius is calculated based on the maximum radius and the radius factor.
 *
 * @param _radius New radius factor for the metaballs.
 */
void MetaballsMode::setRadius(uint8_t _radius)
{
    if (_radius < 1U)
    {
        radiusFactor = 10U;
    }
    else if (_radius > 10U)
    {
        radiusFactor = 1U;
    }
    else
    {
        radiusFactor = 11U - _radius;
    }

    updateRadius();
    /* After changing the radius, we need to clear the display to avoid visual
       artifacts from the previous radius. */
    Display.fillFrame(0U);

    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "radius", static_cast<uint8_t>(radiusFactor));
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
}

void MetaballsMode::updateRadius()
{
    radius = maxRadius / static_cast<float>(radiusFactor);
    radiusSq = radius * radius;
}

/**
 * @brief Publishes the current base speed and ball radius in arbitrary units.
 */
void MetaballsMode::transmit()
{
    JsonDocument doc{};
    doc["speed"].set(speedFactor);
    doc["radius"].set(11U - radiusFactor);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Applies speed and radius from a received payload.
 *
 * @param payload Received configuration fields.
 * @param source Source identifier for the received payload.
 */
void MetaballsMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    if (payload["speed"].is<uint8_t>())
    {
        setSpeed(payload["speed"].as<uint8_t>());
    }
    if (payload["radius"].is<uint8_t>())
    {
        setRadius(payload["radius"].as<uint8_t>());
    }
}

#endif // MODE_METABALLS
