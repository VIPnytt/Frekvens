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
    timerAlarmWrite(timer, 1000000U / (1U << 8) / 50, true);
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

#if EXTENSION_BUILD
void DisplayService::ready()
{
#ifdef FRAME_RATE
    (*Build->config)[Config::h][__STRING(FRAME_RATE)] = FRAME_RATE;
#endif
#ifdef SPI_FREQUENCY
    (*Build->config)[Config::h][__STRING(SPI_FREQUENCY)] = SPI_FREQUENCY;
#endif
}
#endif // EXTENSION_BUILD

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
        tx[(COLUMNS * ROWS + 7) / 8];
    memset(tx, 0, sizeof(tx));
    for (const uint8_t i : Display.pixelBitOrder)
    {
        tx[i >> 3] |= (Display.frameReady[Display.pixelBitOrder[i]] > filter) << (7 - (i & 7));
    }
    ++filter;
#if CONFIG_IDF_TARGET_ESP32
    unsigned char *_tx = (unsigned char *)tx;
    digitalWrite(PIN_CS, LOW);
    SPI.writeBytes(_tx, sizeof(_tx));
    digitalWrite(PIN_CS, HIGH);
#else
    digitalWrite(PIN_CS, LOW);
    SPI.writeBytes(tx, sizeof(tx));
    digitalWrite(PIN_CS, HIGH);
#endif // CONFIG_IDF_TARGET_ESP32
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
        for (const uint8_t i : pixelBitOrder)
        {
            _pixelBitOrder[i] = (COLUMNS - 1 - (pixelBitOrder[i] & (COLUMNS - 1))) + (ROWS - 1 - (pixelBitOrder[i] >> __builtin_ctz(COLUMNS))) * COLUMNS;
        }
        break;
#if COLUMNS == ROWS
    case Orientation::deg90:
        for (const uint8_t i : pixelBitOrder)
        {
            _pixelBitOrder[i] = (pixelBitOrder[i] >> __builtin_ctz(COLUMNS)) + (COLUMNS - 1 - (pixelBitOrder[i] & (COLUMNS - 1))) * COLUMNS;
        }
        break;
    case Orientation::deg270:
        for (const uint8_t i : pixelBitOrder)
        {
            _pixelBitOrder[i] = (ROWS - 1 - (pixelBitOrder[i] >> __builtin_ctz(COLUMNS))) + (pixelBitOrder[i] & (COLUMNS - 1)) * COLUMNS;
        }
        break;
#endif // COLUMNS == ROWS
    default:
        return;
    }
    memcpy(pixelBitOrder, _pixelBitOrder, sizeof(_pixelBitOrder));
    globalOrientation = orientation;
    globalRatio = globalOrientation % 2 ? 1 / ((COLUMNS * CELL_WIDTH) / (float)(ROWS * CELL_HEIGHT)) : (COLUMNS * CELL_WIDTH) / (ROWS * CELL_HEIGHT);

    Preferences Storage;
    Storage.begin(name);
    Storage.putUShort("orientation", globalOrientation);
    Storage.end();

    pending = true;
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
    memcpy(frame, frameReady, sizeof(frameReady));
}

void DisplayService::setFrame(uint8_t frame[COLUMNS * ROWS])
{
    memcpy(frameDraft, frame, sizeof(frameDraft));
}

uint8_t DisplayService::getPixel(uint8_t x, uint8_t y) const
{
#ifdef F_VERBOSE
    if (x >= COLUMNS || y >= ROWS)
    {
        Serial.printf("%s: invalid %d:%d\n", name, x, y);
    }
#endif
    return frameDraft[x + y * COLUMNS];
}

void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t brightness)
{
#ifdef F_VERBOSE
    if (x >= COLUMNS || y >= ROWS)
    {
        Serial.printf("%s: invalid %d:%d\n", name, x, y);
    }
#endif
    frameDraft[x + y * COLUMNS] = brightness;
}

void DisplayService::drawEllipse(double x, double y, double radius, double ratio, bool fill, uint8_t brightness)
{
    double _ratio = (cellRatio * COLUMNS / (double)ROWS * ratio - 1) / 2.0 + 1;
    float rSq = radius * radius;
    uint8_t
        maxX = min(COLUMNS - 1.0, ceil(x + radius / _ratio)),
        maxY = min(ROWS - 1.0, ceil(y + radius * _ratio)),
        minX = max(0.0, floor(x - radius / _ratio)),
        minY = max(0.0, floor(y - radius * _ratio));

    for (uint8_t _x = minX; _x <= maxX; ++_x)
    {
        for (uint8_t _y = minY; _y <= maxY; ++_y)
        {
            float dX = (_x - x) * _ratio;
            float dY = (_y - y) / _ratio;
            float distSq = dX * dX + dY * dY;

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
    doc["pixel"]["ratio"] = PIXEL_SIZE / min(CELL_WIDTH, CELL_HEIGHT);
#endif
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
