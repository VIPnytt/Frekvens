#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MicroFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"

void ModesService::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_mode");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"mode\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::icon] = "mdi:format-list-bulleted";
        component[HomeAssistantAbbreviations::name] = "Mode";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[HomeAssistantAbbreviations::options].to<JsonArray>();
        for (const ModeModule *mode : modes)
        {
            options.add(mode->name);
        }
        component[HomeAssistantAbbreviations::platform] = "select";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.mode}}";
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
    if (!mode)
    {
        setMode(modes[random(modes.size())]);
    }
}

void ModesService::handle()
{
    if (scheduled && millis() - lastMillis > (1 << 11))
    {
        mode = scheduled;
        scheduled = nullptr;
        ESP_LOGI(name, "%s", mode->name);
        mode->begin();
        setActive(true);
        transmit();
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("mode", mode->name);
        Storage.end();
    }
}

void ModesService::onTask(void *parameter)
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
    if (taskHandle && active && eTaskGetState(taskHandle) == eTaskState::eSuspended)
    {
        vTaskResume(taskHandle);
    }
    else if (taskHandle && !active && eTaskGetState(taskHandle) != eTaskState::eSuspended)
    {
        vTaskSuspend(taskHandle);
    }
    else if (!taskHandle && active && mode)
    {
        xTaskCreate(&onTask, name, stackSize, nullptr, 2, &taskHandle);
    }
}

void ModesService::setMode(const char *const name)
{
    if (!mode || strcmp(mode->name, name))
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
    if (taskHandle && this->mode)
    {
        vTaskDelete(taskHandle);
        taskHandle = nullptr;
        this->mode->end();
        this->mode = nullptr;
    }
    lastMillis = millis();
    scheduled = mode;
    const std::string _name = mode->name;
    std::vector<std::string> words = {""};
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
    uint8_t height = 0;
    std::vector<TextHandler> lines;
    for (const std::string &word : words)
    {
        TextHandler line = TextHandler(word, FontMicro);
        height += line.getHeight();
        lines.push_back(line);
        if (height >= GRID_ROWS)
        {
            break;
        }
    }
    const int8_t margin = max<int8_t>(1, (GRID_ROWS - height) / (words.size() + 1));
    uint8_t y = max<int8_t>(0, (GRID_ROWS - height - (words.size() - 1) * margin) / 2);
    Display.clearFrame();
    for (TextHandler &line : lines)
    {
        line.draw((GRID_COLUMNS - min<uint8_t>(GRID_COLUMNS, line.getWidth())) / 2, y);
        y += line.getHeight() + margin;
    }
    Display.flush();
    Display.setPower(power);
}

const std::vector<ModeModule *> &ModesService::getAll() const
{
    return modes;
}

void ModesService::setModeNext()
{
    if (mode)
    {
        const char *const _name = mode->name;
        std::vector<ModeModule *>::const_iterator _modes = std::find_if(modes.begin(), modes.end(), [_name](const ModeModule *_mode)
                                                                        { return !strcmp(_mode->name, _name); });
        if (_modes == modes.end())
        {
            setMode(*modes.begin());
        }
        else if (!Display.getPower())
        {
            Display.setPower(true);
        }
        else
        {
            ++_modes;
            if (_modes == modes.end())
            {
                setMode(*modes.begin());
                Display.setPower(false);
            }
            else
            {
                setMode(*_modes);
            }
        }
    }
    else
    {
        setMode(*modes.begin());
    }
}

void ModesService::setModePrevious()
{
    if (mode)
    {
        const char *const _name = mode->name;
        std::vector<ModeModule *>::const_iterator _modes = std::find_if(modes.begin(), modes.end(), [_name](const ModeModule *_mode)
                                                                        { return !strcmp(_mode->name, _name); });
        if (_modes == modes.begin())
        {
            setMode(*(modes.end() - 1));
            Display.setPower(false);
        }
        else if (!Display.getPower())
        {
            Display.setPower(true);
        }
        else
        {
            setMode(*(_modes - 1));
        }
    }
    else
    {
        setMode(*(modes.end() - 1));
    }
}

void ModesService::transmit()
{
    JsonDocument doc;
    JsonArray list = doc["list"].to<JsonArray>();
    for (const ModeModule *_mode : modes)
    {
        list.add(_mode->name);
    }
    if (mode)
    {
        doc["mode"] = mode->name;
    }
    Device.transmit(doc, name);
}

void ModesService::onReceive(const JsonDocument doc, const char *const source)
{
    // Mode
    if (doc["mode"].is<const char *>())
    {
        setMode(doc["mode"].as<const char *>());
    }
}

ModesService &ModesService::getInstance()
{
    static ModesService instance;
    return instance;
}

ModesService &Modes = Modes.getInstance();
