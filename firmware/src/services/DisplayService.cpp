#include "services/DisplayService.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"            // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/ModesService.h"

#include <SPI.h>
#include <nvs.h>
#include <ranges>

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
    SPI.beginTransaction(
        SPISettings(min<uint32_t>((0b1U << 9U) * GRID_COLUMNS * GRID_ROWS * fps, SPI_FREQUENCY), MSBFIRST, SPI_MODE0));
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
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _brightness{0U};
        if (nvs_get_u8(handle, "brightness", &_brightness) == ESP_OK)
        {
            setBrightness(_brightness);
        }
        uint8_t _orientation{0U};
        if (nvs_get_u8(handle, "orientation", &_orientation) == ESP_OK && _orientation != 0U)
        {
            setOrientation(static_cast<Orientation>(_orientation));
        }
        nvs_close(handle);
    }
    if (brightness == 0U)
    {
        setBrightness(UINT8_MAX);
    }
    if (orientation == Orientation::deg0)
    {
        setOrientation(Orientation::deg0);
    }
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

void DisplayService::flush()
{
    if (!render)
    {
        return;
    }
    std::array<uint16_t, planes.size()> counts{};
    planes[0U].fill(0U);
    for (size_t logical{0U}; logical < frame.size(); ++logical)
    {
        const uint8_t _brightness{frame[logical]};
        if (_brightness != 0U)
        {
#if GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
            const std::pair<uint8_t, uint8_t> &mapping{pixelsMapped[logical]};
            planes[0U][mapping.second] |= static_cast<uint8_t>(mapping.first);
#else
            const std::pair<uint16_t, uint8_t> &mapping{pixelsMapped[logical]};
            planes[0U][mapping.second] |= static_cast<uint16_t>(mapping.first);
#endif // GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
            if (_brightness != UINT8_MAX)
            {
                ++counts[_brightness];
            }
        }
    }
    std::array<uint16_t, planes.size()> offsets{};
    for (uint8_t value{1U}; value < counts.size(); ++value)
    {
        offsets[value] = offsets[value - 1U] + counts[value - 1U];
    }
    std::array<uint16_t, planes.size()> next{offsets};
    std::array<std::pair<uint8_t, uint8_t>, GRID_COLUMNS * GRID_ROWS> indices;
    for (size_t logical{0U}; logical < frame.size(); ++logical)
    {
        const uint8_t _brightness{frame[logical]};
        if (_brightness != 0U && _brightness != UINT8_MAX)
        {
            indices[next[_brightness]++] = pixelsMapped[logical];
        }
    }
    for (uint8_t plane{1U}; plane < planes.size(); ++plane)
    {
        planes[plane] = planes[plane - 1U];
        for (uint16_t idx{offsets[plane]}; idx < offsets[plane] + counts[plane]; ++idx)
        {
            const std::pair<uint8_t, uint8_t> &mapping{indices[idx]};
            planes[plane][mapping.second] &= static_cast<uint8_t>(~mapping.first);
        }
    }
    render = false;
}

float DisplayService::getRatio() const { return ratio; }

DisplayService::Orientation DisplayService::getOrientation() const { return orientation; }

void DisplayService::setOrientation(Orientation _orientation)
{
    switch (_orientation)
    {
    case Orientation::deg0:
        for (uint16_t logical{0U}; logical < pixels.size(); ++logical)
        {
            mapPixel(logical, pixels[logical]);
        }
        break;
#if GRID_COLUMNS == GRID_ROWS
    case Orientation::deg90:
        for (uint16_t logical{0U}; logical < pixels.size(); ++logical)
        {
            mapPixel(
                logical,
                pixels[((GRID_COLUMNS - 1U - (logical % GRID_COLUMNS)) * GRID_COLUMNS) + (logical / GRID_COLUMNS)]);
        }
        break;
#endif // GRID_COLUMNS == GRID_ROWS
    case Orientation::deg180:
        for (uint16_t logical{0U}; logical < pixels.size(); ++logical)
        {
            mapPixel(logical,
                     pixels[(GRID_COLUMNS - 1U - (logical % GRID_COLUMNS)) +
                            (GRID_COLUMNS * (GRID_ROWS - 1U - (logical / GRID_COLUMNS)))]);
        }
        break;
#if GRID_COLUMNS == GRID_ROWS
    case Orientation::deg270:
        for (uint16_t logical{0U}; logical < pixels.size(); ++logical)
        {
            mapPixel(logical,
                     pixels[(GRID_COLUMNS * (logical % GRID_COLUMNS)) + (GRID_ROWS - 1U - (logical / GRID_COLUMNS))]);
        }
        break;
#else
    default:
        return;
#endif // GRID_COLUMNS == GRID_ROWS
    }
    orientation = _orientation;
#if GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    ratio = (static_cast<uint8_t>(orientation) & 0b1U) == 0U ? PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL)
                                                             : PITCH_VERTICAL / static_cast<float>(PITCH_HORIZONTAL);
#endif // GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    render = true;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGI(name.data(), "orientation %u°", static_cast<unsigned>(_orientation) * 90U);
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "orientation", static_cast<uint8_t>(orientation));
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
}

void DisplayService::mapPixel(uint8_t logical, uint8_t physical)
{
#if GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
    pixelsMapped[logical].first = static_cast<uint8_t>(0x80U >> (physical & 7U));
#else
    pixelsMapped[logical].first = static_cast<uint16_t>(0x80U >> (physical & 7U));
#endif // GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
    pixelsMapped[logical].second = static_cast<uint8_t>(physical >> 3U);
}

bool DisplayService::getPower() const { return power; }

void DisplayService::setPower(bool _power)
{
    if (_power && !power)
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
        ESP_LOGI(name.data(), "power on"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        pending = true;
        Modes.setActive(true);
        render = true;
    }
    else if (!_power && power)
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
#endif                                      // SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        ESP_LOGI(name.data(), "power off"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    }
}

uint8_t DisplayService::getBrightness() const { return brightness; }

void DisplayService::setBrightness(uint8_t _brightness)
{
    if (_brightness != brightness || !power)
    {
        ESP_LOGI(name.data(), "brightness"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#ifdef SOC_LEDC_GAMMA_CURVE_FADE_SUPPORTED
        // -2 offset due to `ledcFade` stability issues. Unconfirmed for `ledcFadeGamma`.
        ledcFadeGamma(PIN_OE,
                      power
                          ? max<uint16_t>(brightness,
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
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_u8(handle, "brightness", brightness);
            nvs_commit(handle);
            nvs_close(handle);
        }
        pending = true;
    }
}

uint8_t DisplayService::getPixel(size_t idx) const { return frame[idx]; }

uint8_t DisplayService::getPixel(uint8_t x, uint8_t y) const
{
    return frame[static_cast<size_t>(x + (y * GRID_COLUMNS))];
}

void DisplayService::setPixel(size_t idx, uint8_t _brightness)
{
    frame[idx] = _brightness;
    render = true;
}

void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t _brightness)
{
    frame[static_cast<size_t>(x + (y * GRID_COLUMNS))] = _brightness;
    render = true;
}

void DisplayService::getFrame(std::span<uint8_t, GRID_COLUMNS * GRID_ROWS> _frame) const
{
    std::ranges::copy(frame, _frame.begin());
}

void DisplayService::setFrame(std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS> _frame)
{
    std::ranges::copy(_frame, frame.begin());
    render = true;
}

void DisplayService::drawLineHorizontal(size_t xMin, size_t columns, size_t y, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(xMin + (y * GRID_COLUMNS), columns), _brightness);
    render = true;
}

void DisplayService::drawLineVertical(uint8_t x, uint8_t yMin, uint8_t yMax, uint8_t _brightness)
{
    for (size_t idx{static_cast<size_t>(x + (yMin * GRID_COLUMNS))}; idx <= x + (yMax * GRID_COLUMNS);
         idx += GRID_COLUMNS)
    {
        frame[idx] = _brightness;
    }
    render = true;
}

void DisplayService::drawEllipseOutline(float x, float y, float radius, uint8_t _brightness)
{
#if PITCH_HORIZONTAL == PITCH_VERTICAL
    constexpr float xRatio{static_cast<float>(PITCH_HORIZONTAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
    constexpr float yRatio{static_cast<float>(PITCH_VERTICAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
#else
    constexpr float aRatio{static_cast<float>(PITCH_HORIZONTAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
    constexpr float bRatio{static_cast<float>(PITCH_VERTICAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
    const bool rotated{(static_cast<uint8_t>(orientation) & 0b1U) != 0U};
    const float xRatio{rotated ? bRatio : aRatio};
    const float yRatio{rotated ? aRatio : bRatio};
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    const float radiusSq{radius * radius};
    for (size_t _x{static_cast<size_t>(max(.0F, ceilf(x - (radius / xRatio))))};
         _x <= min(GRID_COLUMNS - 1U, static_cast<unsigned int>(floorf(x + (radius / xRatio))));
         ++_x)
    {
        const float dx{xRatio * (static_cast<float>(_x) - x)};
        const float dy{sqrtf(max(.0F, radiusSq - (dx * dx))) / yRatio};
        const int top{static_cast<int>(ceilf(y - dy))};
        for (int _y{max(0, top - 1)}; _y <= min(static_cast<int>(GRID_ROWS - 1U), top + 1); ++_y)
        {
            const float _dy{yRatio * (static_cast<float>(_y) - y)};
            if (fabsf((dx * dx) + (_dy * _dy) - radiusSq) < radius)
            {
                frame[_x + (static_cast<size_t>(_y) * GRID_COLUMNS)] = _brightness;
            }
        }
        const int bottom{static_cast<int>(floorf(y + dy))};
        for (int _y{max(0, bottom - 1)}; _y <= min(static_cast<int>(GRID_ROWS - 1U), bottom + 1); ++_y)
        {
            const float _dy{yRatio * (static_cast<float>(_y) - y)};
            if (fabsf((dx * dx) + (_dy * _dy) - radiusSq) < radius)
            {
                frame[_x + (static_cast<size_t>(_y) * GRID_COLUMNS)] = _brightness;
            }
        }
    }
    for (size_t _y{static_cast<size_t>(max(.0F, ceilf(y - (radius / yRatio))))};
         _y <= min(GRID_ROWS - 1U, static_cast<unsigned int>(floorf(y + (radius / yRatio))));
         ++_y)
    {
        const float dy{yRatio * (static_cast<float>(_y) - y)};
        const float dx{sqrtf(max(.0F, radiusSq - (dy * dy))) / xRatio};
        const int left{static_cast<int>(ceilf(x - dx))};
        for (int _x{max(0, left - 1)}; _x <= min(static_cast<int>(GRID_COLUMNS - 1U), left + 1); ++_x)
        {
            const float _dx{xRatio * (static_cast<float>(_x) - x)};
            if (fabsf((_dx * _dx) + (dy * dy) - radiusSq) < radius)
            {
                frame[static_cast<size_t>(_x) + (_y * GRID_COLUMNS)] = _brightness;
            }
        }
        const int right{static_cast<int>(floorf(x + dx))};
        for (int _x{max(0, right - 1)}; _x <= min(static_cast<int>(GRID_COLUMNS - 1U), right + 1); ++_x)
        {
            const float _dx{xRatio * (static_cast<float>(_x) - x)};
            if (fabsf((_dx * _dx) + (dy * dy) - radiusSq) < radius)
            {
                frame[static_cast<size_t>(_x) + (_y * GRID_COLUMNS)] = _brightness;
            }
        }
    }
    render = true;
}

void DisplayService::drawEllipseSolid(float x, float y, float radius, uint8_t _brightness)
{
#if PITCH_HORIZONTAL == PITCH_VERTICAL
    constexpr float xRatio{static_cast<float>(PITCH_HORIZONTAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
    constexpr float yRatio{static_cast<float>(PITCH_VERTICAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
#else
    constexpr float aRatio{static_cast<float>(PITCH_HORIZONTAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
    constexpr float bRatio{static_cast<float>(PITCH_VERTICAL * 2U) /
                           static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)};
    const bool rotated{(static_cast<uint8_t>(orientation) & 0b1U) != 0U};
    const float xRatio{rotated ? bRatio : aRatio};
    const float yRatio{rotated ? aRatio : bRatio};
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    const float radiusSq{radius * radius};
    for (size_t _y{static_cast<size_t>(max(.0F, ceilf(y - (radius / yRatio))))};
         _y <= min(GRID_ROWS - 1U, static_cast<unsigned int>(floorf(y + (radius / yRatio))));
         ++_y)
    {
        const float dy{yRatio * (static_cast<float>(_y) - y)};
        const float dx{sqrtf(max(.0F, radiusSq - (dy * dy))) / xRatio};
        const size_t minX{static_cast<size_t>(max(.0F, ceilf(x - dx)))};
        std::ranges::fill(
            std::span{frame}.subspan(static_cast<size_t>(minX + (_y * GRID_COLUMNS)),
                                     min<size_t>(GRID_COLUMNS - 1U, static_cast<size_t>(floorf(x + dx))) - minX + 1U),
            _brightness);
    }
    render = true;
}

void DisplayService::drawRectangleOutline(size_t minX, size_t columns, size_t minY, size_t maxY, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(minX + (minY * GRID_COLUMNS), columns), _brightness);
    std::ranges::fill(std::span{frame}.subspan(minX + (maxY * GRID_COLUMNS), columns), _brightness);
    const size_t maxX{static_cast<size_t>(minX + columns - 1U)};
    for (size_t y{static_cast<size_t>(minY + 1U)}; y < maxY; ++y)
    {
        frame[minX + (y * GRID_COLUMNS)] = _brightness;
        frame[maxX + (y * GRID_COLUMNS)] = _brightness;
    }
    render = true;
}

void DisplayService::drawRectangleSolid(size_t minX, size_t columns, size_t minY, size_t maxY, uint8_t _brightness)
{
    for (size_t y{minY}; y <= maxY; ++y)
    {
        std::ranges::fill(std::span{frame}.subspan(minX + (y * GRID_COLUMNS), columns), _brightness);
    }
    render = true;
}

void DisplayService::fillColumn(uint8_t x, uint8_t _brightness)
{
    for (size_t idx{static_cast<size_t>(x)}; idx < GRID_COLUMNS * GRID_ROWS; idx += GRID_COLUMNS)
    {
        frame[idx] = _brightness;
    }
    render = true;
}

void DisplayService::fillFrame(uint8_t _brightness)
{
    frame.fill(_brightness);
    render = true;
}

void DisplayService::fillRow(size_t y, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(y * GRID_COLUMNS, GRID_COLUMNS), _brightness);
    render = true;
}

void DisplayService::fillRows(size_t minY, size_t rows, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(minY * GRID_COLUMNS, rows * GRID_COLUMNS), _brightness);
    render = true;
}

void DisplayService::invertFrame()
{
    for (uint8_t &_brightness : frame)
    {
        _brightness = UINT8_MAX - _brightness;
    }
    render = true;
}

void DisplayService::transmit()
{
    const bool rotated{(static_cast<uint8_t>(orientation) & 0b1U) != 0U};
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

void DisplayService::onPowerOff()
{
    Display.power = false;
    Display.pending = true;
    Modes.setActive(false);
    for (uint16_t plane{0U}; plane < planes.size(); ++plane)
    {
        planes[plane].fill(0U);
    }
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

IRAM_ATTR void DisplayService::onTimer()
{
    static DRAM_ATTR uint8_t plane{0U};
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), LOW);
    SPI.transferBytes(planes[plane].data(), nullptr, planes[0U].size());
    gpio_set_level(static_cast<gpio_num_t>(PIN_CS), HIGH);
    if (++plane == planes.size())
    {
        plane = 0U;
    }
}

DisplayService &DisplayService::getInstance()
{
    static DRAM_ATTR DisplayService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
DisplayService &Display{DisplayService::getInstance()};
