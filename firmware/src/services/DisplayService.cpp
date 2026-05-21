#include "services/DisplayService.h"

#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"

#include <SPI.h>
#include <nvs.h>

void DisplayService::configure()
{
    pinMode(PIN_CS, OUTPUT);
#ifdef PIN_MISO
    pinMode(PIN_MISO, INPUT);
#endif // PIN_MISO
    pinMode(PIN_MOSI, OUTPUT);
    pinMode(PIN_OE, OUTPUT);
    pinMode(PIN_SCLK, OUTPUT);

#ifdef PIN_MISO
    SPI.begin(PIN_SCLK, PIN_MISO, PIN_MOSI, PIN_CS);
#else
    SPI.begin(PIN_SCLK, GPIO_NUM_NC, PIN_MOSI, PIN_CS);
#endif // PIN_MISO
    SPI.beginTransaction(SPISettings((uint32_t{1U} << 9U) * GRID_COLUMNS * GRID_ROWS * fps, MSBFIRST, SPI_MODE0));

    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    hw_timer_t *timer{timerBegin(static_cast<uint32_t>(planes.size()) * fps)};
    timerAttachInterrupt(timer, &onTimer);
    timerAlarm(timer, 1U, true, 0U);

    ledcAttach(PIN_OE, static_cast<uint32_t>(1.0F / static_cast<float>(0b1U << depth) / PWM_WIDTH), depth);
    ledcOutputInvert(PIN_OE, true);
    ledcWrite(PIN_OE, 0U);
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    ledcSetGammaFactor(GAMMA);
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED

    uint8_t _brightness{UINT8_MAX};
    uint8_t _orientation{static_cast<uint8_t>(Orientation::deg0)};
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        nvs_get_u8(handle, "brightness", &_brightness);
        nvs_get_u8(handle, "orientation", &_orientation);
        nvs_close(handle);
    }
    setOrientation(static_cast<Orientation>(_orientation % 4U));
    setBrightness(_brightness);

    BitmapHandler(splash).draw();
    flush();
}

void DisplayService::handle()
{
    if (pending)
    {
        transmit();
        pending = false;
    }
}

IRAM_ATTR void DisplayService::onTimer()
{
    static DRAM_ATTR size_t plane{0U};
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), LOW);
    SPI.transferBytes(planes[plane].data(), nullptr, planes[0U].size());
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), HIGH);
    if (++plane == planes.size())
    {
        plane = 0U;
    }
}

void DisplayService::flush() // NOLINT(readability-function-cognitive-complexity)
{
    if (!render)
    {
        return;
    }
    size_t idx{0U}; // NOLINT(misc-const-correctness)
    for (size_t byte{0U}; byte < GRID_COLUMNS * GRID_ROWS / 8U; ++byte)
    {
        uint8_t bits{0U};
        if (frame[idx++] != 0U)
        {
            bits |= 0b10000000U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b1000000U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b100000U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b10000U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b1000U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b100U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b10U;
        }
        if (frame[idx++] != 0U)
        {
            bits |= 0b1U;
        }
        planes[0U][byte] = bits;
    }
    if constexpr (GRID_COLUMNS * GRID_ROWS % 8U != 0U)
    {
        uint8_t bits{0U};
        for (size_t bit{0U}; bit < GRID_COLUMNS * GRID_ROWS % 8U; ++bit)
        {
            if (frame[idx++] != 0U)
            {
                bits |= static_cast<uint8_t>(0b10000000U >> bit);
            }
        }
        planes[0U][GRID_COLUMNS * GRID_ROWS / 8U] = bits;
    }
    std::array<size_t, planes.size()> counts{};
    for (size_t idx{0U}; idx < frame.size(); ++idx)
    {
        const uint8_t value{frame[idx]}; // NOLINT(cppcoreguidelines-init-variables)
        if (value != 0U && value != UINT8_MAX)
        {
            ++counts[value];
        }
    }
    std::array<size_t, planes.size()> offsets{};
    for (size_t value{1U}; value < counts.size(); ++value)
    {
        offsets[value] = offsets[value - 1U] + counts[value - 1U];
    }
    std::array<size_t, planes.size()> next{offsets};
    std::array<size_t, GRID_COLUMNS * GRID_ROWS> indices{};
    for (size_t idx{0U}; idx < frame.size(); ++idx)
    {
        const uint8_t value{frame[idx]}; // NOLINT(cppcoreguidelines-init-variables)
        if (value != 0U && value != UINT8_MAX)
        {
            indices[next[value]++] = idx;
        }
    }
    for (size_t plane{1U}; plane < planes.size(); ++plane)
    {
        for (size_t byte{0U}; byte < planes[0U].size(); ++byte)
        {
            planes[plane][byte] = planes[plane - 1U][byte];
        }
        // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
        for (size_t i{offsets[plane]}; i < offsets[plane] + counts[plane]; ++i)
        {
            planes[plane][indices[i] >> 3U] &=
                static_cast<uint8_t>(~static_cast<uint8_t>(0b10000000U >> (indices[i] & 7U)));
        }
    }
    render = false;
}

float DisplayService::getRatio() const { return ratio; }

DisplayService::Orientation DisplayService::getOrientation() const { return orientation; }

void DisplayService::setOrientation(Orientation _orientation)
{
    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> _pixels{};
    switch ((static_cast<uint8_t>(_orientation) + 4U - static_cast<uint8_t>(orientation)) % 4U)
    {
#if GRID_COLUMNS == GRID_ROWS
    case static_cast<uint8_t>(Orientation::deg90):
        for (size_t i{0U}; i < _pixels.size(); ++i)
        {
            _pixels[i] = pixels[((GRID_COLUMNS - 1U - (i % GRID_COLUMNS)) * GRID_COLUMNS) + (i / GRID_COLUMNS)];
        }
        break;
    case static_cast<uint8_t>(Orientation::deg270):
        for (size_t i{0U}; i < _pixels.size(); ++i)
        {
            _pixels[i] = pixels[(GRID_COLUMNS * (i % GRID_COLUMNS)) + (GRID_ROWS - 1U - (i / GRID_COLUMNS))];
        }
        break;
#endif // GRID_COLUMNS == GRID_ROWS
    case static_cast<uint8_t>(Orientation::deg180):
        for (size_t i{0U}; i < _pixels.size(); ++i)
        {
            _pixels[i] = pixels[(GRID_COLUMNS - 1U - (i % GRID_COLUMNS)) +
                                (GRID_COLUMNS * (GRID_ROWS - 1U - (i / GRID_COLUMNS)))];
        }
        break;
    default:
        return;
    }
    std::array<uint8_t, _pixels.size()> _frame{};
    for (size_t i{0U}; i < _frame.size(); ++i)
    {
        _frame[_pixels[i]] = frame[pixels[i]];
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGI("Status", "orientation %d°", static_cast<uint16_t>(_orientation) * 90U);
    pixels = _pixels;
    orientation = _orientation;
    frame = _frame;
    render = true;
#if GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    ratio = (static_cast<uint8_t>(orientation) & 1U) == 0U ? PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL)
                                                           : PITCH_VERTICAL / static_cast<float>(PITCH_HORIZONTAL);
#endif // GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "orientation", static_cast<uint8_t>(orientation));
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
}

bool DisplayService::getPower() const { return power; }

void DisplayService::setPower(bool _power)
{
    ESP_LOGI("Action", "power"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    if (_power)
    {
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ledcFadeGamma(PIN_OE,
                      0U,
                      max<uint16_t>(brightness,
                                    powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                        ((0b1U << depth) - 2U)),
                      (0b1U << 5U) *
                          brightness); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#else
        ledcFade(PIN_OE,
                 0U,
                 max<uint16_t>(brightness,
                               powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                   ((0b1U << depth) - 2U)),
                 (0b1U << 5U) * brightness); // -2 offset due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        power = true;
        pending = true;
        Modes.setActive(true);
        render = true;
    }
    else
    {
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ledcFadeGammaWithInterrupt(
            PIN_OE,
            max<uint16_t>(brightness,
                          powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                              ((0b1U << depth) - 2U)),
            0U,
            (0b1U << 3U) * brightness,
            &onPowerOff); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGammaWithInterrupt`.
#else
        ledcFadeWithInterrupt(
            PIN_OE,
            max<uint16_t>(brightness,
                          powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                              ((0b1U << depth) - 2U)),
            0U,
            (0b1U << 3U) * brightness,
            &onPowerOff); // -2 offset due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    }
}

void DisplayService::onPowerOff()
{
    Display.power = false;
    Display.pending = true;
    Modes.setActive(false);
    for (size_t plane{0U}; plane < planes.size(); ++plane)
    {
        planes[plane].fill(0U);
    }
}

uint8_t DisplayService::getBrightness() const { return brightness; }

void DisplayService::setBrightness(uint8_t _brightness)
{
    ESP_LOGI("Action", "brightness"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
    ledcFadeGamma(PIN_OE,
                  power ? max<uint16_t>(brightness,
                                        powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                            ((0b1U << depth) - 2U))
                        : 0U,
                  max<uint16_t>(_brightness,
                                powf(static_cast<float>(_brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                    ((0b1U << depth) - 2U)),
                  (0b1U << 4U) * abs(brightness - _brightness));
#else
    // -2 offset due to `ledcFade` stability issues.
    ledcFade(PIN_OE,
             power ? max<uint16_t>(brightness,
                                   powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                       ((0b1U << depth) - 2U))
                   : 0U,
             max<uint16_t>(_brightness,
                           powf(static_cast<float>(_brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                               ((0b1U << depth) - 2U)),
             (0b1U << 4U) * abs(brightness - _brightness));
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    if (!power)
    {
        power = true;
        Modes.setActive(true);
        render = true;
    }
    brightness = _brightness;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "brightness", brightness);
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
}

void DisplayService::getFrame(std::span<uint8_t> _frame) const
{
    for (size_t idx{0U}; idx < frame.size(); ++idx)
    {
        _frame[idx] = frame[pixels[idx]];
    }
}

void DisplayService::setFrame(std::span<const uint8_t> _frame)
{
    for (size_t idx{0U}; idx < frame.size(); ++idx)
    {
        frame[pixels[idx]] = _frame[idx];
    }
    render = true;
}

void DisplayService::clearFrame(uint8_t _brightness)
{
    frame.fill(_brightness);
    render = true;
}

void DisplayService::invertFrame()
{
    for (uint8_t &idx : frame)
    {
        idx = UINT8_MAX - idx;
    }
    render = true;
}

uint8_t DisplayService::getPixel(uint8_t x, uint8_t y) const
{
    if (x >= GRID_COLUMNS || y >= GRID_ROWS)
    {
        ESP_LOGV("Device", "invalid pixel %d:%d", x, y); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    }
    return frame[pixels[x + (y * GRID_COLUMNS)]];
}

void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t brightness)
{
    if (x >= GRID_COLUMNS || y >= GRID_ROWS)
    {
        ESP_LOGV("Device", "invalid pixel %d:%d", x, y); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    }
    frame[pixels[x + (y * GRID_COLUMNS)]] = brightness;
    render = true;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void DisplayService::drawEllipse(float x, float y, float radius, float ratio, bool fill, uint8_t brightness)
{
#if PITCH_HORIZONTAL == PITCH_VERTICAL
    const float xRatio{static_cast<float>(PITCH_HORIZONTAL * 2U) /
                       (ratio * static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL))};
    const float yRatio{static_cast<float>(PITCH_VERTICAL * 2U) /
                       (ratio * static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL))};
#else
    const bool rotated{(static_cast<uint8_t>(orientation) & 1U) != 0U};
    const float xRatio{static_cast<float>(2U * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL)) /
                       (ratio * static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL))};
    const float yRatio{static_cast<float>(2U * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL)) /
                       (ratio * static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL))};
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    const uint8_t xMax{
        static_cast<uint8_t>(min<float>(static_cast<float>(GRID_COLUMNS - 1U), ceilf(x + (radius / xRatio))))};
    const uint8_t xMin{static_cast<uint8_t>(max<float>(.0F, floorf(x - (radius / xRatio))))};
    const uint8_t yMax{
        static_cast<uint8_t>(min<float>(static_cast<float>(GRID_ROWS - 1U), ceilf(y + (radius / yRatio))))};
    const uint8_t yMin{static_cast<uint8_t>(max<float>(.0F, floorf(y - (radius / yRatio))))};
    for (size_t _x{xMin}; _x <= xMax; ++_x)
    {
        for (size_t _y{yMin}; _y <= yMax; ++_y)
        {
            const float xDistance{xRatio * (static_cast<float>(_x) - x)};
            const float yDistance{yRatio * (static_cast<float>(_y) - y)};
            const float distance{hypotf(xDistance, yDistance)};
            if (fill ? (distance <= radius) : (fabsf(distance - radius) < .5F))
            {
                setPixel(static_cast<uint8_t>(_x), static_cast<uint8_t>(_y), brightness);
            }
        }
    }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void DisplayService::drawRectangle(uint8_t minX, uint8_t minY, uint8_t maxX, uint8_t maxY, uint8_t brightness)
{
    for (uint16_t x{minX}; x < uint16_t{maxX} + 1U; ++x)
    {
        setPixel(static_cast<uint8_t>(x), minY, brightness);
        if (minY != maxY)
        {
            setPixel(static_cast<uint8_t>(x), maxY, brightness);
        }
    }
    if (minY + 1U < maxY)
    {
        for (uint16_t y{static_cast<uint16_t>(minY + 1U)}; y < uint16_t{maxY}; ++y)
        {
            setPixel(minX, static_cast<uint8_t>(y), brightness);
            if (maxX != minX)
            {
                setPixel(maxX, static_cast<uint8_t>(y), brightness);
            }
        }
    }
}

void DisplayService::transmit()
{
    const bool rotated{(static_cast<uint8_t>(orientation) & 1U) != 0U};
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["brightness"].set(brightness);
#if GRID_COLUMNS == GRID_ROWS
    doc["columns"].set(GRID_COLUMNS);
#else
    doc["columns"].set(rotated ? GRID_ROWS : GRID_COLUMNS);
#endif // GRID_COLUMNS == GRID_ROWS
    doc["orientation"].set(static_cast<uint16_t>(orientation) * 90U);
    doc["power"].set(power);
#if GRID_COLUMNS == GRID_ROWS
    doc["rows"].set(GRID_ROWS);
#else
    doc["rows"].set(rotated ? GRID_COLUMNS : GRID_ROWS);
#endif // GRID_COLUMNS == GRID_ROWS
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void DisplayService::onReceive(JsonObjectConst payload,
                               std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Brightness
    if (payload["brightness"].is<uint8_t>())
    {
        setBrightness(payload["brightness"].as<uint8_t>());
    }
    // Orientation
    if (payload["orientation"].is<uint16_t>())
    {
        setOrientation(
            static_cast<Orientation>(static_cast<uint8_t>((payload["orientation"].as<uint16_t>() / 90U) & 3U)));
    }
    // Power
    if (payload["power"].is<bool>())
    {
        setPower(payload["power"].as<bool>());
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void DisplayService::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_orientation")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"orientation":{{value.replace('°','')}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:rotate-right-variant");
        component[HomeAssistantAbbreviations::name].set("Orientation");
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        options.add("0°");
#if GRID_COLUMNS == GRID_ROWS
        options.add("90°");
#endif // GRID_COLUMNS == GRID_ROWS
        options.add("180°");
#if GRID_COLUMNS == GRID_ROWS
        options.add("270°");
#endif // GRID_COLUMNS == GRID_ROWS
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.orientation}}°");
    }
}
#endif // EXTENSION_HOMEASSISTANT

DisplayService &DisplayService::getInstance()
{
    static DRAM_ATTR DisplayService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
DisplayService &Display{DisplayService::getInstance()};
