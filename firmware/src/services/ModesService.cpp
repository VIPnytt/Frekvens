#include "services/ModesService.h"

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MicroFont.h"      // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <Preferences.h>
#include <memory>

void ModesService::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::string(name).append("_mode")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"mode":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:format-list-bulleted");
        component[HomeAssistantAbbreviations::name].set("Mode");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const ModeModule *mode : modes)
        {
            options.add(mode->name);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.mode}}");
    }
#endif // EXTENSION_HOMEASSISTANT

    for (ModeModule *mode : modes)
    {
        mode->configure();
    }
}

void ModesService::begin()
{
    switch (esp_reset_reason())
    {
    case esp_reset_reason_t::ESP_RST_BROWNOUT:
    case esp_reset_reason_t::ESP_RST_INT_WDT:
    case esp_reset_reason_t::ESP_RST_PANIC:
    case esp_reset_reason_t::ESP_RST_TASK_WDT:
    case esp_reset_reason_t::ESP_RST_WDT:
        setMode(modes[random(modes.size())], false);
        break;
    default:
        Preferences Storage;
        Storage.begin(name, true);
        if (Storage.isKey("mode"))
        {
            const String _mode = Storage.getString("mode");
            Storage.end();
            setMode(_mode.c_str());
        }
        else
        {
            Storage.end();
        }
    }
    if (mode == nullptr)
    {
        setMode(modes[random(modes.size())]);
    }
}

void ModesService::handle()
{
    if (scheduled != nullptr && millis() - lastMillis > (1UL << 11U))
    {
        mode = scheduled;
        scheduled = nullptr;
        ESP_LOGI(name, "%s", mode->name); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mode->begin();
        setActive(true);
        transmit();
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("mode", mode->name);
        Storage.end();
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
        xTaskCreate(&onTask, name, stackSize, nullptr, 2, &taskHandle);
    }
}

void ModesService::setMode(const char *name) // NOLINT(readability-make-member-function-const)
{
    if (mode == nullptr || strcmp(mode->name, name) != 0)
    {
        for (ModeModule *_mode : modes)
        {
            if (!strcmp(_mode->name, name))
            {
                setMode(_mode);
                return;
            }
        }
    }
}

void ModesService::setMode(ModeModule *mode, bool power)
{
    if (taskHandle != nullptr && this->mode != nullptr)
    {
        vTaskDelete(taskHandle);
        taskHandle = nullptr;
        this->mode->end();
        this->mode = nullptr;
    }
    lastMillis = millis();
    scheduled = mode;
    const std::string _name = mode->name;
    std::vector<std::string> words{""};
    uint8_t _line = 0;
    for (std::size_t i = 0; i < _name.length(); ++i)
    {
        switch (_name[i])
        {
        case 0x20: // Space
        case 0x2D: // Hyphen-minus
            words.push_back("");
            ++_line;
            break;
        default:
            words[_line].push_back(_name[i]);
        }
    }
    uint8_t height = 0; // NOLINT(misc-const-correctness)
    std::vector<std::unique_ptr<TextHandler>> lines;
    for (const std::string &word : words)
    {
        std::unique_ptr<TextHandler> handler = std::make_unique<TextHandler>(word, FontMicro);
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
    for (std::unique_ptr<TextHandler> &line : lines)
    {
        line->draw((GRID_COLUMNS - min<uint8_t>(GRID_COLUMNS, line->getWidth())) / 2, y);
        y += line->getHeight() + margin;
    }
    Display.flush();
    Display.setPower(power);
}

const std::vector<ModeModule *> &ModesService::getAll() const { return modes; }

TaskHandle_t ModesService::getTaskHandle() const { return taskHandle; }

void ModesService::setModeNext()
{
    const char *const _name = mode == nullptr ? scheduled->name : mode->name;
    std::vector<ModeModule *>::const_iterator _mode = std::find_if(
        modes.begin(), modes.end(), [_name](const ModeModule *_mode) { return !strcmp(_mode->name, _name); });
    if (!Display.getPower())
    {
        Display.setPower(true);
    }
    else if (_mode == modes.end() - 1)
    {
        setMode(*modes.begin());
        Display.setPower(false);
    }
    else
    {
        setMode(*(_mode + 1));
    }
}

void ModesService::setModePrevious()
{
    const char *const _name = mode == nullptr ? scheduled->name : mode->name;
    std::vector<ModeModule *>::const_iterator _mode = std::find_if(
        modes.begin(), modes.end(), [_name](const ModeModule *_mode) { return !strcmp(_mode->name, _name); });
    if (!Display.getPower())
    {
        Display.setPower(true);
    }
    else if (_mode == modes.begin())
    {
        setMode(*(modes.end() - 1));
        Display.setPower(false);
    }
    else
    {
        setMode(*(_mode - 1));
    }
}

void ModesService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray list{doc["list"].to<JsonArray>()};
    for (const ModeModule *_mode : modes)
    {
        list.add(_mode->name);
    }
    if (mode != nullptr)
    {
        doc["mode"].set(mode->name);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ModesService::onReceive(JsonObjectConst payload,
                             const char *source) // NOLINT(misc-unused-parameters)
{
    // Mode
    if (payload["mode"].is<const char *>())
    {
        setMode(payload["mode"].as<const char *>());
    }
}

ModesService &ModesService::getInstance()
{
    static ModesService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cppcoreguidelines-avoid-non-const-global-variables)
ModesService &Modes = ModesService::getInstance();
