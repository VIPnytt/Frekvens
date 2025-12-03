#include <esp_arduino_version.h> // temporary bugfix mitigation due to ledcFade bug in Arduino 3.3.2 and higher
#include <Preferences.h>
#include <SPI.h>

#include "extensions/HomeAssistantExtension.h"
#include "handlers/BitmapHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

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

    ledcAttach(PIN_OE, 1 / PWM_WIDTH / (float)(1 << depth), depth);
    ledcOutputInvert(PIN_OE, true);
    ledcWrite(PIN_OE, 0);
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    ledcSetGammaFactor(GAMMA);
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED

    Preferences Storage;
    Storage.begin(name, true);
    const uint8_t _brightness = Storage.isKey("brightness") ? Storage.getUShort("brightness") : UINT8_MAX;
    const Orientation _orientation = Storage.isKey("orientation") ? (Orientation)Storage.getUShort("orientation") : orientation;
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
        component[HomeAssistantAbbreviations::command_template] = "{\"orientation\":{{value.replace('°','')}}}";
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
    static uint8_t
        filter = 0,
        bytes[(GRID_COLUMNS * GRID_ROWS + 7) / 8];
    uint8_t
        *frame = Display.frame,
        *out = bytes,
        bitMask = 0x80,
        outByte = 0;
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; i++)
    {
        if (*frame++ > filter)
        {
            outByte |= bitMask;
        }
        if (!(bitMask >>= 1))
        {
            *out++ = outByte;
            outByte = 0;
            bitMask = 0x80;
        }
    }
#if GRID_COLUMNS * GRID_ROWS % 8
    *out = outByte;
#endif // GRID_COLUMNS * GRID_ROWS % 8
    ++filter;
    gpio_set_level((gpio_num_t)PIN_CS, LOW);
    SPI.transferBytes(bytes, nullptr, sizeof(bytes));
    gpio_set_level((gpio_num_t)PIN_CS, HIGH);
}

void DisplayService::flush()
{
    if (memcmp(frame, _frame, sizeof(_frame)))
    {
        memcpy(frame, _frame, sizeof(_frame));
    }
}

float DisplayService::getRatio() const
{
    return ratio;
}

DisplayService::Orientation DisplayService::getOrientation() const
{
    return orientation;
}

void DisplayService::setOrientation(Orientation orientation)
{
    uint8_t _pixel[GRID_COLUMNS * GRID_ROWS];
    switch ((orientation - this->orientation + 4) % 4)
    {
    case Orientation::deg180:
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            _pixel[i] = pixel[((GRID_ROWS - 1 - (i >> __builtin_ctz(GRID_COLUMNS))) << __builtin_ctz(GRID_COLUMNS)) | (GRID_COLUMNS - 1 - (i & (GRID_COLUMNS - 1)))];
        }
        break;
#if GRID_COLUMNS == GRID_ROWS
    case Orientation::deg90:
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            _pixel[i] = pixel[((GRID_COLUMNS - 1 - (i & (GRID_COLUMNS - 1))) << __builtin_ctz(GRID_COLUMNS)) | (i >> __builtin_ctz(GRID_COLUMNS))];
        }
        break;
    case Orientation::deg270:
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            _pixel[i] = pixel[((i & (GRID_COLUMNS - 1)) << __builtin_ctz(GRID_COLUMNS)) | (GRID_ROWS - 1 - (i >> __builtin_ctz(GRID_COLUMNS)))];
        }
        break;
#endif // GRID_COLUMNS == GRID_ROWS
    default:
        return;
    }
    ESP_LOGI(name, "orientation %d°", orientation * 90);
    memcpy(pixel, _pixel, sizeof(_pixel));
    this->orientation = orientation;
#if GRID_COLUMNS == GRID_ROWS
    ratio = this->orientation % 2
                ? PITCH_VERTICAL / (float)PITCH_HORIZONTAL
                : PITCH_HORIZONTAL / (float)PITCH_VERTICAL;
#endif
    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("orientation", this->orientation);
    Storage.end();
    pending = true;
}

bool DisplayService::getPower() const
{
    return power;
}

void DisplayService::setPower(bool power)
{
    if (power == this->power)
    {
        return;
    }
    ESP_LOGI(name, "power");
    if (power)
    {
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ledcFadeGamma(PIN_OE, 0, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1), (1 << 5) * brightness); // offset -3 instead of -1 due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#elif CONFIG_IDF_TARGET_ESP32 && ESP_ARDUINO_VERSION > ESP_ARDUINO_VERSION_VAL(3, 3, 1) && ESP_ARDUINO_VERSION <= ESP_ARDUINO_VERSION_VAL(3, 3, 4)
        // Temporary Arduino bugfix mitigation
#warning "ESP32 classic users: Arduino has a ledcFade crash bug. Downgrade to Arduino 3.3.1 or fading will be disabled."
        ledcWrite(PIN_OE, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 1) + 1));
#else
        ledcFade(PIN_OE, 0, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1), (1 << 5) * brightness); // offset -3 instead of -1 due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        this->power = true;
        pending = true;
        Modes.setActive(true);
    }
    else
    {
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ledcFadeGammaWithInterrupt(PIN_OE, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1), 0, (1 << 3) * brightness, &onPowerOff); // offset -3 instead of -1 due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGammaWithInterrupt`.
#elif CONFIG_IDF_TARGET_ESP32 && ESP_ARDUINO_VERSION > ESP_ARDUINO_VERSION_VAL(3, 3, 1) && ESP_ARDUINO_VERSION <= ESP_ARDUINO_VERSION_VAL(3, 3, 4)
        // Temporary Arduino bugfix mitigation
#warning "ESP32 classic users: Arduino has a ledcFade crash bug. Downgrade to Arduino 3.3.1 or fading will be disabled."
        ledcWrite(PIN_OE, 0);
        onPowerOff();
#else
        ledcFadeWithInterrupt(PIN_OE, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1), 0, (1 << 3) * brightness, &onPowerOff); // offset -3 instead of -1 due to `ledcFade` stability issues.
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

uint8_t DisplayService::getBrightness() const
{
    return brightness;
}

void DisplayService::setBrightness(uint8_t brightness)
{
    if (power && brightness == this->brightness)
    {
        return;
    }
    ESP_LOGI(name, "brightness");
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    ledcFadeGamma(PIN_OE, power ? max<uint16_t>(this->brightness, pow(this->brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1) : 0, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1), (1 << 4) * abs(this->brightness - brightness)); // offset -3 instead of -1 due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
#elif CONFIG_IDF_TARGET_ESP32 && ESP_ARDUINO_VERSION > ESP_ARDUINO_VERSION_VAL(3, 3, 1) && ESP_ARDUINO_VERSION <= ESP_ARDUINO_VERSION_VAL(3, 3, 4)
    // Temporary Arduino bugfix mitigation
#warning "ESP32 classic users: Arduino has a ledcFade crash bug. Downgrade to Arduino 3.3.1 or fading will be disabled."
    ledcWrite(PIN_OE, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 1) + 1));
#else
    ledcFade(PIN_OE, power ? max<uint16_t>(this->brightness, pow(this->brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1) : 0, max<uint16_t>(brightness, pow(brightness / (float)UINT8_MAX, GAMMA) * ((1 << depth) - 3) + 1), (1 << 4) * abs(this->brightness - brightness)); // offset -3 instead of -1 due to `ledcFade` stability issues.
#endif // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
    if (!power)
    {
        power = true;
        Modes.setActive(true);
    }
    this->brightness = brightness;
    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("brightness", this->brightness);
    Storage.end();
    pending = true;
}

void DisplayService::getFrame(uint8_t frame[GRID_COLUMNS * GRID_ROWS])
{
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        frame[i] = this->frame[pixel[i]];
    }
}

void DisplayService::setFrame(uint8_t frame[GRID_COLUMNS * GRID_ROWS])
{
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        this->_frame[pixel[i]] = frame[i];
    }
}

void DisplayService::clearFrame(uint8_t brightness)
{
    memset(_frame, brightness, sizeof(_frame));
}

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
    return frame[pixel[x + y * GRID_COLUMNS]];
}

void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t brightness)
{
    if (x >= GRID_COLUMNS || y >= GRID_ROWS)
    {
        ESP_LOGV(name, "invalid pixel %d:%d", x, y);
    }
    _frame[pixel[x + y * GRID_COLUMNS]] = brightness;
}

void DisplayService::drawEllipse(float x, float y, float radius, float ratio, bool fill, uint8_t brightness)
{
    const bool rotated = orientation % 2;
    const float
        xRatio = 2 * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL) / (ratio * (PITCH_VERTICAL + PITCH_HORIZONTAL)),
        yRatio = 2 * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL) / (ratio * (PITCH_VERTICAL + PITCH_HORIZONTAL));
    const uint8_t
        xMax = min<uint8_t>(GRID_COLUMNS - 1, ceil(x + radius / xRatio)),
        xMin = max<uint8_t>(0, floor(x - radius / xRatio)),
        yMax = min<uint8_t>(GRID_COLUMNS - 1, ceil(y + radius / yRatio)),
        yMin = max<uint8_t>(0, floor(y - radius / yRatio));
    for (uint8_t _x = xMin; _x <= xMax; ++_x)
    {
        for (uint8_t _y = yMin; _y <= yMax; ++_y)
        {
            const float
                xDistance = (_x - x) * xRatio,
                yDistance = (_y - y) * yRatio,
                distance = sqrt(xDistance * xDistance + yDistance * yDistance);
            if (fill ? (distance <= radius) : (fabs(distance - radius) < .5f))
            {
                setPixel(_x, _y, brightness);
            }
        }
    }
}

void DisplayService::drawRectangle(uint8_t minX, uint8_t minY, uint8_t maxX, uint8_t maxY, bool fill, uint8_t brightness)
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
    const bool rotated = orientation % 2;
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
    Device.transmit(doc, name);
}

void DisplayService::onReceive(const JsonDocument doc, const char *const source)
{
    // Brightness
    if (doc["brightness"].is<uint8_t>())
    {
        setBrightness(doc["brightness"].as<uint8_t>());
    }
    // Orientation
    if (doc["orientation"].is<uint16_t>())
    {
        setOrientation((Orientation)(doc["orientation"].as<uint16_t>() / 90));
    }
    // Power
    if (doc["power"].is<bool>())
    {
        setPower(doc["power"].as<bool>());
    }
}

DisplayService &DisplayService::getInstance()
{
    static DRAM_ATTR DisplayService instance;
    return instance;
}

DisplayService &Display = Display.getInstance();
