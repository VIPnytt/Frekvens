#include "services/DisplayService.h"

#include "extensions/HomeAssistantExtension.h"
#include "handlers/BitmapHandler.h"
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
    timerAlarm(timer, 1'000'000 / (1 << 8) / frameRate, true, 0);
    timerStart(timer);

    ledcAttach(PIN_OE, static_cast<uint32_t>(1.0f / PWM_WIDTH / static_cast<float>(1 << depth)), depth);
    ledcOutputInvert(PIN_OE, true);
    ledcWrite(PIN_OE, 0);
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    ledcSetGammaFactor(GAMMA);
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED

    Preferences Storage;
    Storage.begin(name, true);
    const uint8_t _brightness = Storage.isKey("brightness") ? Storage.getUShort("brightness") : UINT8_MAX;
    const Orientation _orientation =
        Storage.isKey("orientation") ? (Orientation)Storage.getUShort("orientation") : orientation;
    Storage.end();
    setOrientation(_orientation);
    setBrightness(_brightness);

    BitmapHandler(hi).draw();
    flush();
}

void DisplayService::begin()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_orientation");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = R"({"orientation":{{value.replace('°','')}}})";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:rotate-right-variant";
        component[HomeAssistantAbbreviations::name] = "Orientation";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[HomeAssistantAbbreviations::options].to<JsonArray>();
        options.add("0°");
#if GRID_COLUMNS == GRID_ROWS
        options.add("90°");
#endif // GRID_COLUMNS == GRID_ROWS
        options.add("180°");
#if GRID_COLUMNS == GRID_ROWS
        options.add("270°");
#endif // GRID_COLUMNS == GRID_ROWS
        component[HomeAssistantAbbreviations::platform] = "select";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.orientation}}°";
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
    static uint8_t filter = 0;
    static uint8_t bytes[((GRID_COLUMNS * GRID_ROWS) + 7) / 8];
    const uint8_t *frame = Display.frame;
    uint16_t outIndex = 0;
    uint8_t bitMask = 0x80;
    uint8_t outByte = 0;
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        if (frame[i] > filter)
        {
            outByte |= bitMask;
        }
        bitMask >>= 1;
        if (bitMask == 0)
        {
            bytes[outIndex++] = outByte;
            outByte = 0;
            bitMask = 0x80;
        }
    }
#if GRID_COLUMNS * GRID_ROWS % 8
    bytes[outIndex] = outByte;
#endif // GRID_COLUMNS * GRID_ROWS % 8
    ++filter;
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), LOW);
    SPI.transferBytes(bytes, nullptr, sizeof(bytes));
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), HIGH);
}

void DisplayService::flush()
{
    if (memcmp(frame, _frame, sizeof(_frame)) != 0)
    {
        memcpy(frame, _frame, sizeof(_frame));
    }
}

float DisplayService::getRatio() const { return ratio; }

DisplayService::Orientation DisplayService::getOrientation() const { return orientation; }

void DisplayService::setOrientation(Orientation _orientation)
{
    std::vector<uint8_t> _pixel(GRID_COLUMNS * GRID_ROWS);
    switch ((_orientation - orientation + 4) % 4)
    {
    case Orientation::deg180:
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            _pixel[i] = pixel[((GRID_ROWS - 1 - (i >> __builtin_ctz(GRID_COLUMNS))) << __builtin_ctz(GRID_COLUMNS)) |
                              (GRID_COLUMNS - 1 - (i & (GRID_COLUMNS - 1)))];
        }
        break;
#if GRID_COLUMNS == GRID_ROWS
    case Orientation::deg90:
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            _pixel[i] = pixel[((GRID_COLUMNS - 1 - (i & (GRID_COLUMNS - 1))) << __builtin_ctz(GRID_COLUMNS)) |
                              (i >> __builtin_ctz(GRID_COLUMNS))];
        }
        break;
    case Orientation::deg270:
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            _pixel[i] = pixel[((i & (GRID_COLUMNS - 1)) << __builtin_ctz(GRID_COLUMNS)) |
                              (GRID_ROWS - 1 - (i >> __builtin_ctz(GRID_COLUMNS)))];
        }
        break;
#endif // GRID_COLUMNS == GRID_ROWS
    default:
        return;
    }
    ESP_LOGI(name, "orientation %d°", _orientation * 90);
    memcpy(pixel, _pixel.data(), _pixel.size());
    orientation = _orientation;
#if GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    ratio = (orientation % 2) == 0 ? PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL)
                                   : PITCH_VERTICAL / static_cast<float>(PITCH_HORIZONTAL);
#endif // GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("orientation", orientation);
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
        ledcFadeGamma(
            PIN_OE,
            0,
            max<uint16_t>(brightness, powf(brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2)),
            (1 << 5) * brightness); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#else
        ledcFade(
            PIN_OE,
            0,
            max<uint16_t>(brightness, powf(brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2)),
            (1 << 5) * brightness); // -2 offset due to `ledcFade` stability issues.
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
            max<uint16_t>(brightness, powf(brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2)),
            0,
            (1 << 3) * brightness,
            &onPowerOff); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGammaWithInterrupt`.
#else
        ledcFadeWithInterrupt(
            PIN_OE,
            max<uint16_t>(brightness, powf(brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2)),
            0,
            (1 << 3) * brightness,
            &onPowerOff); // -2 offset due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    }
}

void DisplayService::onPowerOff()
{
    Display.power = false;
    Display.pending = true;
    Modes.setActive(false);
    memset(Display.frame, 0, sizeof(Display.frame));
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
        power ? max<uint16_t>(brightness, powf(brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2))
              : 0,
        max<uint16_t>(_brightness, powf(_brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2)),
        (1 << 4) * abs(brightness -
                       _brightness)); // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#else
    ledcFade(
        PIN_OE,
        power ? max<uint16_t>(brightness, powf(brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2))
              : 0,
        max<uint16_t>(_brightness, powf(_brightness / static_cast<float>(UINT8_MAX), GAMMA) * ((1 << depth) - 2)),
        (1 << 4) * abs(brightness - _brightness)); // -2 offset due to `ledcFade` stability issues.
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

void DisplayService::getFrame(uint8_t frameCurrent[GRID_COLUMNS * GRID_ROWS])
{
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        frameCurrent[i] = frame[pixel[i]];
    }
}

void DisplayService::setFrame(const uint8_t frameNext[GRID_COLUMNS * GRID_ROWS])
{
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        _frame[pixel[i]] = frameNext[i];
    }
}

void DisplayService::clearFrame(uint8_t brightness) { memset(_frame, brightness, sizeof(_frame)); }

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
    const bool rotated = (orientation % 2) != 0;
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
            const float xDistance = xRatio * (_x - x);
            const float yDistance = yRatio * (_y - y);
            const float distance = sqrtf((xDistance * xDistance) + (yDistance * yDistance));
            if (fill ? (distance <= radius) : (fabsf(distance - radius) < .5f))
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
    const bool rotated = (orientation % 2) != 0;
    JsonDocument doc;
    doc["brightness"] = brightness;
#if GRID_COLUMNS == GRID_ROWS
    doc["columns"] = GRID_COLUMNS;
#else
    doc["columns"] = rotated ? GRID_ROWS : GRID_COLUMNS;
#endif // GRID_COLUMNS == GRID_ROWS
    doc["orientation"] = orientation * 90;
    doc["power"] = power;
#if GRID_COLUMNS == GRID_ROWS
    doc["rows"] = GRID_ROWS;
#else
    doc["rows"] = rotated ? GRID_COLUMNS : GRID_ROWS;
#endif // GRID_COLUMNS == GRID_ROWS
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void DisplayService::onReceive(JsonObjectConst payload, const char *source)
{
    // Brightness
    if (payload["brightness"].is<uint8_t>())
    {
        setBrightness(payload["brightness"].as<uint8_t>());
    }
    // Orientation
    if (payload["orientation"].is<uint16_t>())
    {
        setOrientation((Orientation)(payload["orientation"].as<uint16_t>() / 90));
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

DisplayService &Display = Display.getInstance();
