#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MicroFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"

void ModesService::setup()
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
        for (const ModeModule *mode : modules)
        {
            options.add(mode->name);
        }
        component[HomeAssistantAbbreviations::platform] = "select";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.mode}}";
    }
#endif // EXTENSION_HOMEASSISTANT

    for (ModeModule *mode : modules)
    {
        mode->setup();
    }
    ESP_LOGV(name, "setup complete");
}

void ModesService::ready()
{
    switch (esp_reset_reason())
    {
    case esp_reset_reason_t::ESP_RST_BROWNOUT:
    case esp_reset_reason_t::ESP_RST_INT_WDT:
    case esp_reset_reason_t::ESP_RST_PANIC:
    case esp_reset_reason_t::ESP_RST_TASK_WDT:
    case esp_reset_reason_t::ESP_RST_WDT:
        setMode(modules[random(modules.size())]->name, name);
        Display.setPower(false, name);
        break;
    default:
        Preferences Storage;
        Storage.begin(name, true);
        if (Storage.isKey("active"))
        {
            const String _active = Storage.getString("active");
            Storage.end();
            setMode(_active.c_str(), name);
        }
        else
        {
            Storage.end();
            setMode(modules[random(modules.size())]->name, name);
        }
    }
}

void ModesService::handle()
{
    if (scheduled && millis() - lastMillis > (1 << 11))
    {
        active->wake();
        setActive(true);
        transmit();
        scheduled = false;
    }
}

void ModesService::onTask(void *parameter)
{
    for (;;)
    {
        if (Display.getPower())
        {
            Modes.active->handle();
            Display.flush();
        }
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
    else if (!taskHandle && active && this->active)
    {
        xTaskCreate(&onTask, name, stackSize, nullptr, 2, &taskHandle);
    }
}

void ModesService::setMode(const char *const name, const char *const source)
{
    if (!active || strcmp(active->name, name))
    {
        for (ModeModule *mode : modules)
        {
            if (!strcmp(mode->name, name))
            {
                setMode(mode, source);
                return;
            }
        }
    }
}

void ModesService::setMode(ModeModule *mode, const char *const source)
{
    if (taskHandle && active)
    {
        vTaskDelete(taskHandle);
        taskHandle = nullptr;
        active->sleep();
    }
    active = mode;
    ESP_LOGI(source, "%s", active->name);

    Preferences Storage;
    Storage.begin(name);
    Storage.putString("active", active->name);
    Storage.end();

    lastMillis = millis();
    scheduled = true;
    splash();
    Display.setPower(true, source);
}

void ModesService::splash()
{
    const std::string _name = active->name;
    std::vector<std::string> lines = {""};
    uint8_t _line = 0;
    for (std::size_t i = 0; i < _name.length(); ++i)
    {
        switch (_name[i])
        {
        case 0x20: //   Space
        case 0x2D: // - Hyphen-minus
            lines.push_back("");
            ++_line;
            break;
        default:
            lines[_line].push_back(_name[i]);
        }
    }
    uint8_t height = 0;
    std::vector<TextHandler> texts;
    for (const std::string &line : lines)
    {
        TextHandler text(line, FontMicro);
        height += text.getHeight();
        texts.push_back(text);
        if (height >= GRID_ROWS)
        {
            break;
        }
    }
    const int8_t margin = max<int8_t>(1, (GRID_ROWS - height) / (lines.size() + 1));
    uint8_t y = max<int8_t>(0, (GRID_ROWS - height - (lines.size() - 1) * margin) / 2);
    Display.clearFrame();
    for (TextHandler &text : texts)
    {
        text.draw((GRID_COLUMNS - min<uint8_t>(GRID_COLUMNS, text.getWidth())) / 2, y);
        y += text.getHeight() + margin;
    }
    Display.flush();
}

const std::vector<ModeModule *> &ModesService::getAll() const
{
    return modules;
}

void ModesService::setModeNext(const char *const source)
{
    if (active)
    {
        const char *const _name = active->name;
        std::vector<ModeModule *>::const_iterator _modules = std::find_if(modules.begin(), modules.end(), [_name](const ModeModule *_mode)
                                                                          { return !strcmp(_mode->name, _name); });
        if (_modules == modules.end())
        {
            setMode((*modules.begin())->name, source);
        }
        else if (!Display.getPower())
        {
            Display.setPower(true, name);
        }
        else
        {
            ++_modules;
            if (_modules == modules.end())
            {
                setMode((*modules.begin())->name, source);
                Display.setPower(false, name);
            }
            else
            {
                setMode((*_modules)->name, source);
            }
        }
    }
    else
    {
        setMode((*modules.begin())->name, source);
    }
}

void ModesService::setModePrevious(const char *const source)
{
    if (active)
    {
        const char *const _name = active->name;
        std::vector<ModeModule *>::const_iterator _modules = std::find_if(modules.begin(), modules.end(), [_name](const ModeModule *_mode)
                                                                          { return !strcmp(_mode->name, _name); });
        if (_modules == modules.begin())
        {
            setMode((*(modules.end() - 1))->name, source);
            Display.setPower(false, name);
        }
        else if (!Display.getPower())
        {
            Display.setPower(true, source);
        }
        else
        {
            setMode((*(_modules - 1))->name, source);
        }
    }
    else
    {
        setMode((*(modules.end() - 1))->name, source);
    }
}

void ModesService::transmit()
{
    JsonDocument doc;
    JsonArray list = doc["list"].to<JsonArray>();
    for (const ModeModule *mode : modules)
    {
        list.add(mode->name);
    }
    if (active)
    {
        doc["mode"] = active->name;
    }
    Device.transmit(doc, name);
}

void ModesService::receiverHook(const JsonDocument doc, const char *const source)
{
    // Mode
    if (doc["mode"].is<const char *>())
    {
        setMode(doc["mode"].as<const char *>(), source);
    }
}

ModesService &ModesService::getInstance()
{
    static ModesService instance;
    return instance;
}

ModesService &Modes = Modes.getInstance();
