#include "services/ModesService.h"

#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"

#include <nvs.h>
#include <vector>

void ModesService::configure()
{
    for (const std::string_view _name : names)
    {
        getMode(_name)->configure();
    }
}

void ModesService::begin()
{
    const esp_reset_reason_t reason = esp_reset_reason();
    if (std::ranges::any_of(Device.resetAbnormalities, [&](esp_reset_reason_t _reason) { return _reason == reason; }))
    {
        setMode(names[random(names.size())], false);
    }
    else
    {
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            size_t len{0};
            if (nvs_get_str(handle, "mode", nullptr, &len) == ESP_OK && len > 0)
            {
                std::vector<char> _mode(len);
                nvs_get_str(handle, "mode", _mode.data(), &len);
                nvs_close(handle);
                setMode(_mode.data());
            }
            else
            {
                nvs_close(handle);
            }
        }
    }
    if (mode == nullptr)
    {
        setMode(names[random(names.size())]);
    }
}

void ModesService::handle()
{
    if (scheduled && millis() - lastMillis > (1UL << 11U))
    {
        scheduled = false;
        ESP_LOGI(
            "Mode", "%s", std::string(mode->name).c_str()); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mode->begin();
        setActive(true);
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_str(handle, "mode", std::string(mode->name).c_str());
            nvs_commit(handle);
            nvs_close(handle);
        }
        transmit();
    }
}

void ModesService::onTask(void *parameter) // NOLINT(misc-unused-parameters)
{
    for (;;)
    {
        Modes.mode->handle();
        Display.flush();
        vTaskDelay(1);
    }
}

void ModesService::setActive(bool active)
{
    if (taskHandle != nullptr && active && eTaskGetState(taskHandle) == eTaskState::eSuspended)
    {
        vTaskResume(taskHandle);
    }
    else if (taskHandle != nullptr && !active && eTaskGetState(taskHandle) != eTaskState::eSuspended)
    {
        vTaskSuspend(taskHandle);
    }
    else if (taskHandle == nullptr && active && mode != nullptr)
    {
        xTaskCreate(&onTask, "Mode", stackSize, nullptr, 1, &taskHandle);
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
std::unique_ptr<ModeModule> ModesService::getMode(std::string_view modeName)
{
#if MODE_ANIMATION
    if (modeName == AnimationMode::name)
    {
        return std::make_unique<AnimationMode>();
    }
#endif
#if MODE_ARROW
    if (modeName == ArrowMode::name)
    {
        return std::make_unique<ArrowMode>();
    }
#endif
#if MODE_BINARYCLOCK
    if (modeName == BinaryClockMode::name)
    {
        return std::make_unique<BinaryClockMode>();
    }
#endif
#if MODE_BINARYEPOCH
    if (modeName == BinaryEpochMode::name)
    {
        return std::make_unique<BinaryEpochMode>();
    }
#endif
#if MODE_BLINDS
    if (modeName == BlindsMode::name)
    {
        return std::make_unique<BlindsMode>();
    }
#endif
#if MODE_BLINK
    if (modeName == BlinkMode::name)
    {
        return std::make_unique<BlinkMode>();
    }
#endif
#if MODE_BREAKOUTCLOCK
    if (modeName == BreakoutClockMode::name)
    {
        return std::make_unique<BreakoutClockMode>();
    }
#endif
#if MODE_BRIGHT
    if (modeName == BrightMode::name)
    {
        return std::make_unique<BrightMode>();
    }
#endif
#if MODE_CIRCLE
    if (modeName == CircleMode::name)
    {
        return std::make_unique<CircleMode>();
    }
#endif
#if MODE_CLOCK
    if (modeName == ClockMode::name)
    {
        return std::make_unique<ClockMode>();
    }
#endif
#if MODE_COUNTDOWN
    if (modeName == CountdownMode::name)
    {
        return std::make_unique<CountdownMode>();
    }
#endif
#if MODE_DRAW
    if (modeName == DrawMode::name)
    {
        return std::make_unique<DrawMode>();
    }
#endif
#if MODE_EQUALIZER
    if (modeName == EqualizerMode::name)
    {
        return std::make_unique<EqualizerMode>();
    }
#endif
#if MODE_FIREWORK
    if (modeName == FireworkMode::name)
    {
        return std::make_unique<FireworkMode>();
    }
#endif
#if MODE_FLIES
    if (modeName == FliesMode::name)
    {
        return std::make_unique<FliesMode>();
    }
#endif
#if MODE_GAMEOFLIFE
    if (modeName == GameOfLifeMode::name)
    {
        return std::make_unique<GameOfLifeMode>();
    }
#endif
#if MODE_GLITTER
    if (modeName == GlitterMode::name)
    {
        return std::make_unique<GlitterMode>();
    }
#endif
#if MODE_HOMETHERMOMETER
    if (modeName == HomeThermometerMode::name)
    {
        return std::make_unique<HomeThermometerMode>();
    }
#endif
#if MODE_JAGGEDWAVEFORM
    if (modeName == JaggedWaveformMode::name)
    {
        return std::make_unique<JaggedWaveformMode>();
    }
#endif
#if MODE_LEAFFALL
    if (modeName == LeafFallMode::name)
    {
        return std::make_unique<LeafFallMode>();
    }
#endif
#if MODE_LINES
    if (modeName == LinesMode::name)
    {
        return std::make_unique<LinesMode>();
    }
#endif
#if MODE_METABALLS
    if (modeName == MetaballsMode::name)
    {
        return std::make_unique<MetaballsMode>();
    }
#endif
#if MODE_NOISE
    if (modeName == NoiseMode::name)
    {
        return std::make_unique<NoiseMode>();
    }
#endif
#if MODE_PINGPONG
    if (modeName == PingPongMode::name)
    {
        return std::make_unique<PingPongMode>();
    }
#endif
#if MODE_PIXELSEQUENCE
    if (modeName == PixelSequenceMode::name)
    {
        return std::make_unique<PixelSequenceMode>();
    }
#endif
#if MODE_RAIN
    if (modeName == RainMode::name)
    {
        return std::make_unique<RainMode>();
    }
#endif
#if MODE_RING
    if (modeName == RingMode::name)
    {
        return std::make_unique<RingMode>();
    }
#endif
#if MODE_SCAN
    if (modeName == ScanMode::name)
    {
        return std::make_unique<ScanMode>();
    }
#endif
#if MODE_SMOOTHWAVEFORM
    if (modeName == SmoothWaveformMode::name)
    {
        return std::make_unique<SmoothWaveformMode>();
    }
#endif
#if MODE_SNAKE
    if (modeName == SnakeMode::name)
    {
        return std::make_unique<SnakeMode>();
    }
#endif
#if MODE_STARS
    if (modeName == StarsMode::name)
    {
        return std::make_unique<StarsMode>();
    }
#endif
#if MODE_STREAM
    if (modeName == StreamMode::name)
    {
        return std::make_unique<StreamMode>();
    }
#endif
#if MODE_TICKER
    if (modeName == TickerMode::name)
    {
        return std::make_unique<TickerMode>();
    }
#endif
#if MODE_WAVEFORM
    if (modeName == WaveformMode::name)
    {
        return std::make_unique<WaveformMode>();
    }
#endif
#if MODE_WEATHER
    if (modeName == WeatherMode::name)
    {
        return std::make_unique<WeatherMode>();
    }
#endif
    return nullptr;
}

void ModesService::setMode(std::string_view modeName, bool power)
{
    if (std::unique_ptr<ModeModule> _mode{getMode(modeName)})
    {
        if (mode)
        {
            setActive(false);
            mode->end();
            mode.reset();
        }
        mode = std::move(_mode);
        lastMillis = millis();
        scheduled = true;
        if (power)
        {
            Display.setPower(power);
        }
        const std::string _name{mode->name};
        std::vector<std::string> words{""};
        uint8_t _line = 0;
        for (std::size_t i = 0; i < _name.length(); ++i)
        {
            switch (_name[i])
            {
            case 0x20: // SPACE
            case 0x2D: // -
                words.push_back("");
                ++_line;
                break;
            default:
                words[_line].push_back(_name[i]);
            }
        }
        uint8_t height = 0; // NOLINT(misc-const-correctness)
        std::vector<std::unique_ptr<TextHandler>> lines;
#if FONT_MICRO
        const std::unique_ptr<const FontModule> font{Fonts.get(MicroFont::name)};
#else
        const std::unique_ptr<const FontModule> font{Fonts.get(Fonts.names[0])};
#endif // FONT_MICRO
        for (const std::string &word : words)
        {
            std::unique_ptr<TextHandler> handler{std::make_unique<TextHandler>(word, *font)};
            const uint8_t lineHeight = handler->getHeight();
            if (height + lineHeight >= GRID_ROWS)
            {
                break;
            }
            height += lineHeight;
            lines.emplace_back(std::move(handler));
        }
        const int8_t margin = max<int8_t>(1, (GRID_ROWS - height) / (lines.size() + 1));
        uint8_t y = max<int8_t>(0, (GRID_ROWS - height - (lines.size() - 1) * margin) / 2);
        Display.clearFrame();
        for (const std::unique_ptr<TextHandler> &line : lines)
        {
            line->draw((GRID_COLUMNS - min<uint8_t>(GRID_COLUMNS, line->getWidth())) / 2, y);
            y += line->getHeight() + margin;
        }
        Display.flush();
    }
}

ModeModule *ModesService::getMode() { return mode.get(); }

TaskHandle_t ModesService::getTaskHandle() const { return taskHandle; }

void ModesService::setModeNext()
{
    if (!Display.getPower())
    {
        Display.setPower(true);
    }
    for (std::size_t i = 0; i < names.size(); ++i)
    {
        if (names[i] == mode->name)
        {
            const size_t index = (i + 1) % names.size();
            setMode(names[index], index != 0);
            return;
        }
    }
}

void ModesService::setModePrevious()
{
    if (!Display.getPower())
    {
        Display.setPower(true);
    }
    for (std::size_t i = 0; i < names.size(); ++i)
    {
        if (names[i] == mode->name)
        {
            const size_t index = (i - 1) % names.size();
            setMode(names[index], index != 0);
            return;
        }
    }
}

void ModesService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray list{doc["list"].to<JsonArray>()};
    for (const std::string_view _name : names)
    {
        list.add(_name);
    }
    if (mode != nullptr)
    {
        doc["mode"].set(mode->name);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ModesService::onReceive(JsonObjectConst payload,
                             std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Mode
    if (payload["mode"].is<std::string_view>())
    {
        setMode(payload["mode"].as<std::string_view>());
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ModesService::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_mode")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"mode":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:format-list-bulleted");
        component[HomeAssistantAbbreviations::name].set("Mode");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const std::string_view _name : names)
        {
            options.add(_name);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.mode}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

ModesService &ModesService::getInstance()
{
    static ModesService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
ModesService &Modes = ModesService::getInstance();
