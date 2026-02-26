#include "services/DisplayService.h"

#include "extensions/HomeAssistantExtension.h"
#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/ModesService.h"

#include <Preferences.h>
#include <SPI.h>

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
    SPI.beginTransaction(SPISettings(INT16_MAX * GRID_COLUMNS * GRID_ROWS, MSBFIRST, SPI_MODE0));

    timer = timerBegin(1'000'000);
    timerAttachInterrupt(timer, &onTimer);
    timerAlarm(timer, 1'000'000 / (1U << 8U) / frameRate, true, 0);
    timerStart(timer);

    ledcAttach(PIN_OE, static_cast<uint32_t>(1.0F / PWM_WIDTH / static_cast<float>(1U << depth)), depth);
    ledcOutputInvert(PIN_OE, true);
    ledcWrite(PIN_OE, 0);
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    ledcSetGammaFactor(GAMMA);
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED

    Preferences Storage;
    Storage.begin(name, true);
    const uint8_t _brightness = Storage.isKey("brightness") ? Storage.getUShort("brightness") : UINT8_MAX;
    const Orientation _orientation =
        Storage.isKey("orientation") ? static_cast<Orientation>(Storage.getUShort("orientation")) : orientation;
    Storage.end();
    setOrientation(_orientation);
    setBrightness(_brightness);

    BitmapHandler(splash).draw();
    flush();
}

void DisplayService::begin()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::string(name).append("_orientation")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"orientation":{{value.replace('°','')}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:rotate-right-variant");
        component[HomeAssistantAbbreviations::name].set("Orientation");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
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
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.orientation}}°");
    }
#endif // EXTENSION_HOMEASSISTANT
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
    static DRAM_ATTR std::array<uint8_t, ((GRID_COLUMNS * GRID_ROWS) + 7) / 8> bytes{};
    static DRAM_ATTR uint8_t threshold = 0;
    size_t pixel = 0;
    for (size_t i = 0; i < GRID_COLUMNS * GRID_ROWS / 8; ++i)
    {
        uint8_t byte = 0;
        byte |= (Display.frame[pixel++] > threshold) ? 0x80U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x40U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x20U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x10U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x08U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x04U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x02U : 0U;
        byte |= (Display.frame[pixel++] > threshold) ? 0x01U : 0U;
        bytes[i] = byte;
    }
    if constexpr (GRID_COLUMNS * GRID_ROWS % 8 != 0)
    {
        uint8_t byte = 0;
        for (size_t remainder = 0; remainder < GRID_COLUMNS * GRID_ROWS % 8; ++remainder)
        {
            byte |= (Display.frame[pixel++] > threshold) ? (0x80U >> remainder) : 0U;
        }
        bytes[GRID_COLUMNS * GRID_ROWS / 8] = byte;
    }
    threshold += 1;
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), LOW);
    SPI.transferBytes(bytes.data(), nullptr, bytes.size());
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), HIGH);
}

void DisplayService::flush()
{
    if (frame != _frame)
    {
        frame = _frame;
    }
}

float DisplayService::getRatio() const { return ratio; }

DisplayService::Orientation DisplayService::getOrientation() const { return orientation; }

void DisplayService::setOrientation(Orientation _orientation)
{
    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> _pixel{};
    switch ((static_cast<uint8_t>(_orientation) + 4U - static_cast<uint8_t>(orientation)) % 4U)
    {
#if GRID_COLUMNS == GRID_ROWS
    case static_cast<uint8_t>(Orientation::deg90):
        for (std::size_t i = 0; i < _pixel.size(); ++i)
        {
            _pixel[i] = pixel[((GRID_COLUMNS - 1 - (i & (GRID_COLUMNS - 1))) << __builtin_ctz(GRID_COLUMNS)) |
                              (i >> __builtin_ctz(GRID_COLUMNS))];
        }
        break;
    case static_cast<uint8_t>(Orientation::deg270):
        for (std::size_t i = 0; i < _pixel.size(); ++i)
        {
            _pixel[i] = pixel[((i & (GRID_COLUMNS - 1)) << __builtin_ctz(GRID_COLUMNS)) |
                              (GRID_ROWS - 1 - (i >> __builtin_ctz(GRID_COLUMNS)))];
        }
        break;
#endif // GRID_COLUMNS == GRID_ROWS
    case static_cast<uint8_t>(Orientation::deg180):
        for (std::size_t i = 0; i < _pixel.size(); ++i)
        {
            _pixel[i] = pixel[((GRID_ROWS - 1 - (i >> __builtin_ctz(GRID_COLUMNS))) << __builtin_ctz(GRID_COLUMNS)) |
                              (GRID_COLUMNS - 1 - (i & (GRID_COLUMNS - 1)))];
        }
        break;
    default:
        return;
    }
    ESP_LOGI(name, "orientation %d°", static_cast<uint8_t>(_orientation) * 90U);
    pixel = _pixel;
    orientation = _orientation;
#if GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    ratio = (static_cast<uint8_t>(orientation) % 2U) == 0 ? PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL)
                                                          : PITCH_VERTICAL / static_cast<float>(PITCH_HORIZONTAL);
#endif // GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("orientation", static_cast<uint8_t>(orientation));
    Storage.end();
    pending = true;
}

bool DisplayService::getPower() const { return power; }

void DisplayService::setPower(bool _power)
{
    if (_power == power)
    {
        return;
    }
    ESP_LOGI(name, "power");
    if (_power)
    {
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ledcFadeGamma(PIN_OE,
                      0,
                      max<uint16_t>(brightness,
                                    powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                        ((1U << depth) - 2)),
                      (1U << 5U) *
                          brightness); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#else
        ledcFade(PIN_OE,
                 0,
                 max<uint16_t>(brightness,
                               powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                   ((1U << depth) - 2)),
                 (1U << 5U) * brightness); // -2 offset due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        power = true;
        pending = true;
        Modes.setActive(true);
    }
    else
    {
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ledcFadeGammaWithInterrupt(
            PIN_OE,
            max<uint16_t>(brightness,
                          powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                              ((1U << depth) - 2)),
            0,
            (1U << 3U) * brightness,
            &onPowerOff); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGammaWithInterrupt`.
#else
        ledcFadeWithInterrupt(
            PIN_OE,
            max<uint16_t>(brightness,
                          powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                              ((1U << depth) - 2)),
            0,
            (1U << 3U) * brightness,
            &onPowerOff); // -2 offset due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    }
}

void DisplayService::onPowerOff()
{
    Display.power = false;
    Display.pending = true;
    Modes.setActive(false);
    Display.frame.fill(0);
}

uint8_t DisplayService::getBrightness() const { return brightness; }

void DisplayService::setBrightness(uint8_t _brightness)
{
    if (power && _brightness == brightness)
    {
        return;
    }
    if (_brightness == 0)
    {
        setPower(false);
        return;
    }
    ESP_LOGI(name, "brightness");
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    ledcFadeGamma(
        PIN_OE,
        power ? max<uint16_t>(brightness,
                              powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                  ((1U << depth) - 2))
              : 0,
        max<uint16_t>(_brightness,
                      powf(static_cast<float>(_brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                          ((1U << depth) - 2)),
        (1U << 4U) *
            abs(brightness -
                _brightness)); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#else
    ledcFade(PIN_OE,
             power ? max<uint16_t>(brightness,
                                   powf(static_cast<float>(brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                                       ((1U << depth) - 2))
                   : 0,
             max<uint16_t>(_brightness,
                           powf(static_cast<float>(_brightness) / static_cast<float>(UINT8_MAX), GAMMA) *
                               ((1U << depth) - 2)),
             (1U << 4U) * abs(brightness - _brightness)); // -2 offset due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    if (!power)
    {
        power = true;
        Modes.setActive(true);
    }
    brightness = _brightness;
    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("brightness", brightness);
    Storage.end();
    pending = true;
}

void DisplayService::getFrame(std::span<uint8_t> frameCurrent) const
{
    for (size_t i = 0; i < frameCurrent.size(); ++i)
    {
        frameCurrent[i] = frame[pixel[i]];
    }
}

void DisplayService::setFrame(std::span<const uint8_t> frameNext)
{
    for (size_t i = 0; i < frameNext.size(); ++i)
    {
        _frame[pixel[i]] = frameNext[i];
    }
}

void DisplayService::clearFrame(uint8_t brightness) { _frame.fill(brightness); }

void DisplayService::invertFrame()
{
    for (uint8_t &pixel : _frame)
    {
        pixel = UINT8_MAX - pixel;
    }
}

uint8_t DisplayService::getPixel(uint8_t x, uint8_t y) const
{
    if (x >= GRID_COLUMNS || y >= GRID_ROWS)
    {
        ESP_LOGV(name, "invalid pixel %d:%d", x, y);
    }
    return frame[pixel[x + (y * GRID_COLUMNS)]];
}

void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t brightness)
{
    if (x >= GRID_COLUMNS || y >= GRID_ROWS)
    {
        ESP_LOGV(name, "invalid pixel %d:%d", x, y);
    }
    _frame[pixel[x + (y * GRID_COLUMNS)]] = brightness;
}

void DisplayService::drawEllipse(float x, float y, float radius, float ratio, bool fill, uint8_t brightness)
{
#if PITCH_HORIZONTAL == PITCH_VERTICAL
    const float xRatio =
        static_cast<float>(2 * PITCH_HORIZONTAL) / (ratio * static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL));
    const float yRatio =
        static_cast<float>(2 * PITCH_VERTICAL) / (ratio * static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL));
#else
    const bool rotated = (static_cast<uint8_t>(orientation) % 2) != 0;
    const float xRatio = static_cast<float>(2 * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL)) /
                         (ratio * (PITCH_VERTICAL + PITCH_HORIZONTAL));
    const float yRatio = static_cast<float>(2 * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL)) /
                         (ratio * (PITCH_VERTICAL + PITCH_HORIZONTAL));
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    const uint8_t xMax = min<uint8_t>(GRID_COLUMNS - 1, ceilf(x + (radius / xRatio)));
    const uint8_t xMin = max<uint8_t>(0, floorf(x - (radius / xRatio)));
    const uint8_t yMax = min<uint8_t>(GRID_COLUMNS - 1, ceilf(y + (radius / yRatio)));
    const uint8_t yMin = max<uint8_t>(0, floorf(y - (radius / yRatio)));
    for (uint16_t _x = xMin; _x <= xMax; ++_x)
    {
        for (uint16_t _y = yMin; _y <= yMax; ++_y)
        {
            const float xDistance = xRatio * (static_cast<float>(_x) - x);
            const float yDistance = yRatio * (static_cast<float>(_y) - y);
            const float distance = sqrtf((xDistance * xDistance) + (yDistance * yDistance));
            if (fill ? (distance <= radius) : (fabsf(distance - radius) < .5F))
            {
                setPixel(_x, _y, brightness);
            }
        }
    }
}

void DisplayService::drawRectangle(uint8_t minX, uint8_t minY, uint8_t maxX, uint8_t maxY, bool fill,
                                   uint8_t brightness)
{
    for (uint8_t x = minX; x <= maxX; ++x)
    {
        for (uint8_t y = minY; y <= maxY; ++y)
        {
            if (fill || x == minX || y == minY || x == maxX || y == maxY)
            {
                setPixel(x, y, brightness);
            }
        }
    }
}

void DisplayService::transmit()
{
    const bool rotated = (static_cast<uint8_t>(orientation) % 2U) != 0U;
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["brightness"].set(brightness);
#if GRID_COLUMNS == GRID_ROWS
    doc["columns"].set(GRID_COLUMNS);
#else
    doc["columns"].set(rotated ? GRID_ROWS : GRID_COLUMNS);
#endif // GRID_COLUMNS == GRID_ROWS
    doc["orientation"].set(static_cast<uint8_t>(orientation) * 90U);
    doc["power"].set(power);
#if GRID_COLUMNS == GRID_ROWS
    doc["rows"].set(GRID_ROWS);
#else
    doc["rows"].set(rotated ? GRID_COLUMNS : GRID_ROWS);
#endif // GRID_COLUMNS == GRID_ROWS
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void DisplayService::onReceive(JsonObjectConst payload,
                               const char *source) // NOLINT(misc-unused-parameters)
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

DisplayService &DisplayService::getInstance()
{
    static DRAM_ATTR DisplayService instance;
    return instance;
}

// NOLINTNEXTLINE(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
DisplayService &Display = DisplayService::getInstance();
