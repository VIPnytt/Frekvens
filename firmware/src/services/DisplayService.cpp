#include "services/DisplayService.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"            // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/ModesService.h"

#include <SPI.h>
#include <nvs.h>
#include <ranges>

/**
 * @brief Initializes the display hardware and restores persisted display settings.
 *
 * Configures the SPI interface, refresh timer, PWM output, brightness, and orientation,
 * then renders and flushes the splash screen.
 */
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

/**
 * @brief Transmits the current display state when an update is pending.
 */
void DisplayService::handle()
{
    if (pending)
    {
        transmit();
        pending = false;
    }
}

/**
 * @brief Converts the pending frame into brightness planes for transmission.
 *
 * Skips conversion when rendering is not pending and clears the pending state
 * after conversion.
 */
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
            const std::pair<size_t, size_t> &mapping{pixelsMapped[logical]};
            planes[0U][mapping.second] |= mapping.first;
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

/**
 * @brief Gets the display orientation.
 *
 * @return Orientation The current display orientation.
 */
DisplayService::Orientation DisplayService::getOrientation() const { return orientation; }

/**
 * @brief Applies a supported display orientation and updates its pixel mapping.
 *
 * Unsupported orientations are ignored on non-square displays. The selected
 * orientation is persisted, the display aspect ratio is updated when needed,
 * and rendering and device-state transmission are scheduled.
 *
 * @param _orientation Orientation to apply.
 */
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

/**
 * @brief Maps a logical pixel to its physical byte offset and bit mask.
 *
 * @param logical Logical pixel index.
 * @param physical Physical pixel index.
 */
void DisplayService::mapPixel(size_t logical, size_t physical)
{
    pixelsMapped[logical].first = static_cast<size_t>(0x80U >> (physical & 7U));
    pixelsMapped[logical].second = static_cast<size_t>(physical >> 3U);
}

/**
 * @brief Gets the display power state.
 *
 * @return `true` if the display is powered on, `false` otherwise.
 */
bool DisplayService::getPower() const { return power; }

/**
 * @brief Fades the display on or off.
 *
 * Activating the display enables display modes and schedules rendering.
 * Deactivating it fades the output to zero before completing shutdown.
 *
 * @param _power Whether the display should be powered on.
 */
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

/**
 * @brief Gets the display brightness.
 *
 * @return uint8_t Current display brightness.
 */
uint8_t DisplayService::getBrightness() const { return brightness; }

/**
 * @brief Sets the display brightness and enables the display when necessary.
 *
 * @param _brightness Desired brightness level.
 */
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

/**
 * @brief Gets the brightness of a pixel by its frame index.
 *
 * @param idx Pixel index in the frame.
 * @return uint8_t Brightness value of the specified pixel.
 */
uint8_t DisplayService::getPixel(size_t idx) const { return frame[idx]; }

/**
 * @brief Retrieves the brightness of a pixel at the specified coordinates.
 *
 * @param x Horizontal pixel coordinate.
 * @param y Vertical pixel coordinate.
 * @return uint8_t Pixel brightness value.
 */
uint8_t DisplayService::getPixel(uint8_t x, uint8_t y) const
{
    return frame[static_cast<size_t>(x + (y * GRID_COLUMNS))];
}

/**
 * @brief Sets the brightness of a pixel by its frame index.
 *
 * @param idx Frame index of the pixel.
 * @param _brightness Pixel brightness value.
 */
void DisplayService::setPixel(size_t idx, uint8_t _brightness)
{
    frame[idx] = _brightness;
    render = true;
}

/**
 * @brief Sets the brightness of a pixel at the specified coordinates.
 *
 * @param x Horizontal pixel coordinate.
 * @param y Vertical pixel coordinate.
 * @param _brightness Pixel brightness value.
 */
void DisplayService::setPixel(uint8_t x, uint8_t y, uint8_t _brightness)
{
    frame[static_cast<size_t>(x + (y * GRID_COLUMNS))] = _brightness;
    render = true;
}

/**
 * @brief Copies the current display frame into the specified buffer.
 *
 * @param _frame Destination buffer for the frame brightness values.
 */
void DisplayService::getFrame(std::span<uint8_t, GRID_COLUMNS * GRID_ROWS> _frame) const
{
    std::ranges::copy(frame, _frame.begin());
}

/**
 * @brief Replaces the display frame with the specified pixel brightness values.
 *
 * @param _frame Brightness values for each display pixel.
 */
void DisplayService::setFrame(std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS> _frame)
{
    std::ranges::copy(_frame, frame.begin());
    render = true;
}

/**
 * @brief Draws a horizontal line at the specified row.
 *
 * @param xMin Starting column.
 * @param columns Number of columns to fill.
 * @param y Row containing the line.
 * @param _brightness Brightness value applied to each pixel.
 */
void DisplayService::drawLineHorizontal(size_t xMin, size_t columns, size_t y, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(xMin + (y * GRID_COLUMNS), columns), _brightness);
    render = true;
}

/**
 * @brief Draws a vertical line at the specified column.
 *
 * @param x Column coordinate.
 * @param yMin Starting row coordinate, inclusive.
 * @param yMax Ending row coordinate, inclusive.
 * @param _brightness Brightness value applied to each pixel.
 */
void DisplayService::drawLineVertical(uint8_t x, uint8_t yMin, uint8_t yMax, uint8_t _brightness)
{
    for (size_t idx{static_cast<size_t>(x + (yMin * GRID_COLUMNS))}; idx <= x + (yMax * GRID_COLUMNS);
         idx += GRID_COLUMNS)
    {
        frame[idx] = _brightness;
    }
    render = true;
}

/**
 * @brief Draws an ellipse outline on the frame.
 *
 * The ellipse is adjusted for display pitch and orientation, and clipped to
 * the display boundaries.
 *
 * @param x Horizontal center coordinate.
 * @param y Vertical center coordinate.
 * @param radius Ellipse radius.
 * @param _brightness Brightness assigned to the outline pixels.
 */
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
        const float xDistance{xRatio * (static_cast<float>(_x) - x)};
        const float yDistance{sqrtf(max(.0F, radiusSq - (xDistance * xDistance))) / yRatio};
        const int top{static_cast<int>(ceilf(y - yDistance))};
        for (int _y{max(0, top - 1)}; _y <= min(static_cast<int>(GRID_ROWS - 1U), top + 1); ++_y)
        {
            const float _yDistance{yRatio * (static_cast<float>(_y) - y)};
            if (fabsf((xDistance * xDistance) + (_yDistance * _yDistance) - radiusSq) < radius)
            {
                frame[_x + (static_cast<size_t>(_y) * GRID_COLUMNS)] = _brightness;
            }
        }
        const int bottom{static_cast<int>(floorf(y + yDistance))};
        for (int _y{max(0, bottom - 1)}; _y <= min(static_cast<int>(GRID_ROWS - 1U), bottom + 1); ++_y)
        {
            const float _dy{yRatio * (static_cast<float>(_y) - y)};
            if (fabsf((xDistance * xDistance) + (_dy * _dy) - radiusSq) < radius)
            {
                frame[_x + (static_cast<size_t>(_y) * GRID_COLUMNS)] = _brightness;
            }
        }
    }
    for (size_t _y{static_cast<size_t>(max(.0F, ceilf(y - (radius / yRatio))))};
         _y <= min(GRID_ROWS - 1U, static_cast<unsigned int>(floorf(y + (radius / yRatio))));
         ++_y)
    {
        const float yDistance{yRatio * (static_cast<float>(_y) - y)};
        const float xDistance{sqrtf(max(.0F, radiusSq - (yDistance * yDistance))) / xRatio};
        const int left{static_cast<int>(ceilf(x - xDistance))};
        for (int _x{max(0, left - 1)}; _x <= min(static_cast<int>(GRID_COLUMNS - 1U), left + 1); ++_x)
        {
            const float _xDistance{xRatio * (static_cast<float>(_x) - x)};
            if (fabsf((_xDistance * _xDistance) + (yDistance * yDistance) - radiusSq) < radius)
            {
                frame[static_cast<size_t>(_x) + (_y * GRID_COLUMNS)] = _brightness;
            }
        }
        const int right{static_cast<int>(floorf(x + xDistance))};
        for (int _x{max(0, right - 1)}; _x <= min(static_cast<int>(GRID_COLUMNS - 1U), right + 1); ++_x)
        {
            const float _xDistance{xRatio * (static_cast<float>(_x) - x)};
            if (fabsf((_xDistance * _xDistance) + (yDistance * yDistance) - radiusSq) < radius)
            {
                frame[static_cast<size_t>(_x) + (_y * GRID_COLUMNS)] = _brightness;
            }
        }
    }
    render = true;
}

/**
 * @brief Fills the interior of an ellipse centered at the specified coordinates.
 *
 * @param x Horizontal coordinate of the ellipse center.
 * @param y Vertical coordinate of the ellipse center.
 * @param radius Ellipse radius.
 * @param _brightness Brightness value for pixels inside the ellipse.
 */
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
        const float yDistance{yRatio * (static_cast<float>(_y) - y)};
        const float xDistance{sqrtf(max(.0F, radiusSq - (yDistance * yDistance))) / xRatio};
        const size_t minX{static_cast<size_t>(max(.0F, ceilf(x - xDistance)))};
        std::ranges::fill(std::span{frame}.subspan(
                              static_cast<size_t>(minX + (_y * GRID_COLUMNS)),
                              min<size_t>(GRID_COLUMNS - 1U, static_cast<size_t>(floorf(x + xDistance))) - minX + 1U),
                          _brightness);
    }
    render = true;
}

/**
 * @brief Draws a rectangular outline on the frame.
 *
 * @param minX Leftmost column of the rectangle.
 * @param columns Number of columns in the rectangle.
 * @param minY Top row of the rectangle.
 * @param maxY Bottom row of the rectangle.
 * @param _brightness Brightness applied to the outline pixels.
 */
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

/**
 * @brief Fills a rectangular region of the frame with a brightness value.
 *
 * @param minX Starting column of the rectangle.
 * @param columns Number of columns to fill.
 * @param minY Starting row of the rectangle.
 * @param maxY Ending row of the rectangle, inclusive.
 * @param _brightness Brightness value applied to each pixel.
 */
void DisplayService::drawRectangleSolid(size_t minX, size_t columns, size_t minY, size_t maxY, uint8_t _brightness)
{
    for (size_t y{minY}; y <= maxY; ++y)
    {
        std::ranges::fill(std::span{frame}.subspan(minX + (y * GRID_COLUMNS), columns), _brightness);
    }
    render = true;
}

/**
 * @brief Fills a display column with the specified brightness.
 *
 * @param x Column index to fill.
 * @param _brightness Brightness value assigned to each pixel in the column.
 */
void DisplayService::fillColumn(uint8_t x, uint8_t _brightness)
{
    for (size_t idx{static_cast<size_t>(x)}; idx < GRID_COLUMNS * GRID_ROWS; idx += GRID_COLUMNS)
    {
        frame[idx] = _brightness;
    }
    render = true;
}

/**
 * @brief Fills the entire frame with the specified brightness.
 *
 * @param _brightness Brightness value applied to every pixel.
 */
void DisplayService::fillFrame(uint8_t _brightness)
{
    frame.fill(_brightness);
    render = true;
}

/**
 * @brief Fills a row of the frame with the specified brightness.
 *
 * @param y Row index to fill.
 * @param _brightness Brightness value applied to each pixel in the row.
 */
void DisplayService::fillRow(size_t y, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(y * GRID_COLUMNS, GRID_COLUMNS), _brightness);
    render = true;
}

/**
 * @brief Fills consecutive rows of the frame with a brightness value.
 *
 * @param minY Index of the first row to fill.
 * @param rows Number of consecutive rows to fill.
 * @param _brightness Brightness value assigned to each pixel.
 */
void DisplayService::fillRows(size_t minY, size_t rows, uint8_t _brightness)
{
    std::ranges::fill(std::span{frame}.subspan(minY * GRID_COLUMNS, rows * GRID_COLUMNS), _brightness);
    render = true;
}

/**
 * @brief Inverts the brightness of every pixel in the frame.
 */
void DisplayService::invertFrame()
{
    for (uint8_t &_brightness : frame)
    {
        _brightness = UINT8_MAX - _brightness;
    }
    render = true;
}

/**
 * @brief Transmits the display's current state and dimensions.
 */
void DisplayService::transmit()
{
    const bool rotated{(static_cast<uint8_t>(orientation) & 0b1U) != 0U};
    JsonDocument doc{};
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
/**
 * @brief Adds Home Assistant discovery metadata for display orientation control.
 *
 * @param discovery Discovery document to which the orientation component is added.
 * @param topic Base MQTT topic for the display.
 * @param unique Prefix used to construct the component's unique identifier.
 */
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
        component[HomeAssistantAbbreviations::options][0U].set("0°");
#if GRID_COLUMNS == GRID_ROWS
        component[HomeAssistantAbbreviations::options][1U].set("90°");
        component[HomeAssistantAbbreviations::options][2U].set("180°");
        component[HomeAssistantAbbreviations::options][3U].set("270°");
#else
        component[HomeAssistantAbbreviations::options][1U].set("180°");
#endif // GRID_COLUMNS == GRID_ROWS
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.orientation}}°");
    }
}
#endif // EXTENSION_HOMEASSISTANT

/**
 * @brief Handles display power-off state changes.
 *
 * Clears all brightness planes, deactivates display modes, and schedules a
 * device state transmission.
 */
void DisplayService::onPowerOff()
{
    Display.power = false;
    Display.pending = true;
    Modes.setActive(false);
    for (auto &plane : planes)
    {
        plane.fill(0U);
    }
}

/**
 * @brief Applies supported display settings from a JSON payload.
 *
 * @param payload JSON object containing optional brightness, orientation, and power settings.
 */
void DisplayService::onReceive(JsonObjectConst payload, std::string_view source)
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

/**
 * @brief Transmits the next brightness plane to the display over SPI.
 */
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

/**
 * @brief Returns the shared display service instance.
 *
 * @return DisplayService& Reference to the singleton display service.
 */
DisplayService &DisplayService::getInstance()
{
    static DRAM_ATTR DisplayService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
DisplayService &Display{DisplayService::getInstance()};
