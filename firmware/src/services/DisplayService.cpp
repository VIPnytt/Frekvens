#include <Preferences.h>
#include <regex>
#include <soc/rtc.h>
#include <SPI.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "handlers/BitmapHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

void DisplayService::setup()
{
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_EN, OUTPUT);
#ifdef PIN_MISO
    pinMode(PIN_MISO, INPUT);
#endif
    pinMode(PIN_MOSI, OUTPUT);
    pinMode(PIN_SCLK, OUTPUT);

#ifdef PIN_MISO
    SPI.begin(PIN_SCLK, PIN_MISO, PIN_MOSI, PIN_CS);
#else
    SPI.begin(PIN_SCLK, GPIO_NUM_NC, PIN_MOSI, PIN_CS);
#endif // PIN_MISO

#ifdef SPI_FREQUENCY
    SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
#else
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
#endif // SPI_FREQUENCY

    timer = timerBegin(0, rtc_clk_apb_freq_get() / 1000000U, true);
    timerAttachInterrupt(timer, &onTimer, true);

#ifdef FRAME_RATE
    timerAlarmWrite(timer, 1000000U / (1U << 8) / FRAME_RATE, true);
#else
    timerAlarmWrite(timer, 1000000U / (1U << 8) / 60, true);
#endif // FRAME_RATE

    timerAlarmEnable(timer);
    timerStart(timer);

    ledcSetup(0, 1 / PWM_WIDTH / (double)(1U << PWM_DEPTH), PWM_DEPTH);

    uint8_t brightness = globalBrightness;
    Orientation orientation = globalOrientation;

    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("brightness"))
    {
        brightness = Storage.getUShort("brightness");
    }
    if (Storage.isKey("orientation"))
    {
        orientation = (Orientation)Storage.getUShort("orientation");
    }
    Storage.end();
    setGlobalOrientation(orientation);
    setGlobalBrightness(brightness);

    BitmapHandler(hi).draw();
    flush();

#if defined(F_VERBOSE) && defined(SPI_FREQUENCY)
    Serial.printf("%s: %.0f fps @ %.1f MHz\n", name, 1000000U / (float)(1U << 8) / (float)timerAlarmRead(timer), SPI_FREQUENCY / (float)1000000);
#elif defined(F_DEBUG)
    Serial.printf("%s: %.0f fps\n", name, 1000000U / (float)(1U << 8) / (float)timerAlarmRead(timer));
#endif
}

void DisplayService::ready()
{
#if EXTENSION_BUILD && defined(FRAME_RATE)
    (*Build->config)[Config::h][__STRING(FRAME_RATE)] = FRAME_RATE;
#endif
#if EXTENSION_BUILD && defined(SPI_FREQUENCY)
    (*Build->config)[Config::h][__STRING(SPI_FREQUENCY)] = SPI_FREQUENCY;
#endif

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_orientation");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"orientation\":{{value.replace('°','')}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:rotate-right-variant";
        component[Abbreviations::name] = "Orientation";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[Abbreviations::options].to<JsonArray>();
        options.add("0°");
#if COLUMNS == ROWS
        options.add("90°");
#endif // COLUMNS == ROWS
        options.add("180°");
#if COLUMNS == ROWS
        options.add("270°");
#endif // COLUMNS == ROWS
        component[Abbreviations::platform] = "select";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.orientation}}°";
    }
#endif // EXTENSION_BUILD || EXTENSION_HOMEASSISTANT
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
        data[(COLUMNS * ROWS + 7) / 8];
    uint8_t
        *frame = Display.frameReady,
        *out = data,
        bitMask = 0x80,
        outByte = 0;
    for (uint16_t i = 0; i < COLUMNS * ROWS; i++)
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
#if COLUMNS * ROWS % 8
    *out = outByte;
#endif // COLUMNS * ROWS % 8
    ++filter;
    gpio_set_level((gpio_num_t)PIN_CS, 0);
    SPI.transferBytes(data, nullptr, sizeof(data));
    gpio_set_level((gpio_num_t)PIN_CS, 1);
}

void DisplayService::flush()
{
    if (memcmp(frameReady, frameDraft, sizeof(frameDraft)))
    {
        memcpy(frameReady, frameDraft, sizeof(frameDraft));
    }
}

double DisplayService::getCellRatio() const
{
    return cellRatio;
}

DisplayService::Orientation DisplayService::getGlobalOrientation() const
{
    return globalOrientation;
}

void DisplayService::setGlobalOrientation(Orientation orientation)
{
    uint8_t _pixelBitOrder[COLUMNS * ROWS];
    switch ((orientation - globalOrientation + 4) % 4)
    {
    case Orientation::deg180:
        for (uint16_t i = 0; i < COLUMNS * ROWS; ++i)
        {
            _pixelBitOrder[i] = pixelBitOrder[((ROWS - 1 - (i >> __builtin_ctz(COLUMNS))) << __builtin_ctz(COLUMNS)) | (COLUMNS - 1 - (i & (COLUMNS - 1)))];
        }
        break;
#if COLUMNS == ROWS
    case Orientation::deg90:
        for (uint16_t i = 0; i < COLUMNS * ROWS; ++i)
        {
            _pixelBitOrder[i] = pixelBitOrder[((COLUMNS - 1 - (i & (COLUMNS - 1))) << __builtin_ctz(COLUMNS)) | (i >> __builtin_ctz(COLUMNS))];
        }
        break;
    case Orientation::deg270:
        for (uint16_t i = 0; i < COLUMNS * ROWS; ++i)
        {
            _pixelBitOrder[i] = pixelBitOrder[((i & (COLUMNS - 1)) << __builtin_ctz(COLUMNS)) | (ROWS - 1 - (i >> __builtin_ctz(COLUMNS)))];
        }
        break;
#endif // COLUMNS == ROWS
    default:
        return;
    }
    memcpy(pixelBitOrder, _pixelBitOrder, sizeof(_pixelBitOrder));
    globalOrientation = orientation;
#if COLUMNS == ROWS
    globalRatio = globalOrientation % 2
                      ? ROWS * CELL_HEIGHT / (float)COLUMNS / (float)CELL_WIDTH
                      : COLUMNS * CELL_WIDTH / (float)ROWS / (float)CELL_HEIGHT;
    cellRatio = globalOrientation % 2
                    ? CELL_HEIGHT / (float)CELL_WIDTH
                    : CELL_WIDTH / (float)CELL_HEIGHT;
#endif
    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("orientation", globalOrientation);
    Storage.end();
    pending = true;
    if (Modes.active)
    {
        Modes.active->wake();
    }
}

bool DisplayService::getPower() const
{
    return power;
}

void DisplayService::setPower(bool state)
{
    if (state == power)
    {
        return;
    }
    power = state;
    if (power)
    {
        ledcAttachPin(PIN_EN, 0);
    }
    else
    {
        ledcDetachPin(PIN_EN);
        digitalWrite(PIN_EN, HIGH);
    }
    pending = true;
}

uint8_t DisplayService::getGlobalBrightness() const
{
    return globalBrightness;
}

void DisplayService::setGlobalBrightness(uint8_t brightness)
{
    if (power && brightness == globalBrightness)
    {
        return;
    }
    power = true;
    globalBrightness = brightness;
    ledcAttachPin(PIN_EN, 0);
    ledcWrite(0, (1U << SOC_LEDC_TIMER_BIT_WIDE_NUM) - 1 - max((uint16_t)globalBrightness, (uint16_t)(pow(globalBrightness / (double)UINT8_MAX, GAMMA) * ((1U << SOC_LEDC_TIMER_BIT_WIDE_NUM) - 2) + 1)));

    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("brightness", globalBrightness);
    Storage.end();

    pending = true;
}

void DisplayService::clear(uint8_t brightness)
{
    memset(frameDraft, brightness, sizeof(frameDraft));
}

void DisplayService::invert()
{
    for (uint8_t &pixel : frameDraft)
    {
        pixel = UINT8_MAX - pixel;
    }
}

void DisplayService::getFrame(uint8_t frame[COLUMNS * ROWS])
{
    for (uint16_t i = 0; i < COLUMNS * ROWS; i++)
    {
        frame[i] = frameReady[pixelBitOrder[i]];
    }
}

void DisplayService::setFrame(uint8_t frame[COLUMNS * ROWS])
{
    for (uint16_t i = 0; i < COLUMNS * ROWS; i++)
    {
        frameDraft[pixelBitOrder[i]] = frame[i];
    }
}

uint8_t DisplayService::getPixel(uint8_t x, uint8_t y) const
{
#ifdef F_VERBOSE
    if (x >= COLUMNS || y >= ROWS)
    {
        Serial.printf("%s: invalid %d:%d\n", name, x, y);
    }
#endif
    return frameReady[pixelBitOrder[x + y * COLUMNS]];
}

void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t brightness)
{
#ifdef F_VERBOSE
    if (x >= COLUMNS || y >= ROWS)
    {
        Serial.printf("%s: invalid %d:%d\n", name, x, y);
    }
#endif
    frameDraft[pixelBitOrder[x + y * COLUMNS]] = brightness;
}

void DisplayService::drawEllipse(double x, double y, double radius, double ratio, bool fill, uint8_t brightness)
{
    const double _ratio = (cellRatio * COLUMNS / (double)ROWS * ratio - 1) / 2.0 + 1;
    const float rSq = radius * radius;
    const uint8_t
        xMax = min(COLUMNS - 1.0, ceil(x + radius / _ratio)),
        yMax = min(ROWS - 1.0, ceil(y + radius * _ratio)),
        xMin = max(0.0, floor(x - radius / _ratio)),
        yMin = max(0.0, floor(y - radius * _ratio));

    for (uint8_t _x = xMin; _x <= xMax; ++_x)
    {
        for (uint8_t _y = yMin; _y <= yMax; ++_y)
        {
            const float
                xDist = (_x - x) * _ratio,
                yDist = (_y - y) / _ratio,
                distSq = xDist * xDist + yDist * yDist;

            if (fill && distSq <= rSq)
            {
                setPixel(_x, _y, brightness);
            }
            else if (fabs(distSq - rSq) <= radius)
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
    JsonDocument doc;
    doc["brightness"] = globalBrightness;
    doc["orientation"] = globalOrientation * 90;
#if COLUMNS == ROWS
    doc["pixel"]["columns"] = COLUMNS;
    doc["pixel"]["rows"] = ROWS;
#else
    doc["pixel"]["columns"] = globalOrientation % 2 ? COLUMNS : ROWS;
    doc["pixel"]["rows"] = globalOrientation % 2 ? ROWS : COLUMNS;
#endif // COLUMNS == ROWS
#if CELL_WIDTH != CELL_HEIGHT
    doc["pixel"]["ratio"] = PIXEL_SIZE / (float)min(CELL_WIDTH, CELL_HEIGHT);
#endif // CELL_WIDTH != CELL_HEIGHT
    doc["power"] = power;
    doc["ratio"] = globalRatio;
    Device.transmit(doc, name);
}

void DisplayService::receiverHook(const JsonDocument doc)
{
    // Brightness
    if (doc["brightness"].is<uint8_t>())
    {
        setGlobalBrightness(doc["brightness"].as<uint8_t>());
    }
    // Orientation
    if (doc["orientation"].is<uint16_t>())
    {
        setGlobalOrientation((Orientation)(doc["orientation"].as<uint16_t>() % 360 / 90 % 4));
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
