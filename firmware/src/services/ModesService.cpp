#include <Preferences.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "fonts/MicroFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"

void ModesService::setup()
{
#if EXTENSION_BUILD && defined(TASK_STACK_MODES)
    (*Build->config)[Config::h][__STRING(TASK_STACK_MODES)] = TASK_STACK_MODES;
#endif
#if EXTENSION_BUILD && defined(MODE_ANIMATION) && !(MODE_ANIMATION)
    (*Build->config)[Config::h][__STRING(MODE_ANIMATION)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_ARROW) && !(MODE_ARROW)
    (*Build->config)[Config::h][__STRING(MODE_ARROW)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_ARTNET) && !(MODE_ARTNET)
    (*Build->config)[Config::h][__STRING(MODE_ARTNET)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BINARYCLOCK) && !(MODE_BINARYCLOCK)
    (*Build->config)[Config::h][__STRING(MODE_BINARYCLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BINARYEPOCH) && !(MODE_BINARYEPOCH)
    (*Build->config)[Config::h][__STRING(MODE_BINARYEPOCH)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BLINK) && !(MODE_BLINK)
    (*Build->config)[Config::h][__STRING(MODE_BLINK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BLINDS) && !(MODE_BLINDS)
    (*Build->config)[Config::h][__STRING(MODE_BLINDS)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BOLDCLOCK) && !(MODE_BOLDCLOCK)
    (*Build->config)[Config::h][__STRING(MODE_BOLDCLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BREAKOUTCLOCK) && !(MODE_BREAKOUTCLOCK)
    (*Build->config)[Config::h][__STRING(MODE_BREAKOUTCLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_BRIGHT) && !(MODE_BRIGHT)
    (*Build->config)[Config::h][__STRING(MODE_BRIGHT)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_CIRCLE) && !(MODE_CIRCLE)
    (*Build->config)[Config::h][__STRING(MODE_CIRCLE)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_COUNTDOWN) && !(MODE_COUNTDOWN)
    (*Build->config)[Config::h][__STRING(MODE_COUNTDOWN)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_DISTRIBUTEDDISPLAYPROTOCOL) && !(MODE_DISTRIBUTEDDISPLAYPROTOCOL)
    (*Build->config)[Config::h][__STRING(MODE_DISTRIBUTEDDISPLAYPROTOCOL)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_DRAW) && !(MODE_DRAW)
    (*Build->config)[Config::h][__STRING(MODE_DRAW)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_E131) && !(MODE_E131)
    (*Build->config)[Config::h][__STRING(MODE_E131)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_EQUALIZER) && !(MODE_EQUALIZER)
    (*Build->config)[Config::h][__STRING(MODE_EQUALIZER)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_FIREWORK) && !(MODE_FIREWORK)
    (*Build->config)[Config::h][__STRING(MODE_FIREWORK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_FLIES) && !(MODE_FLIES)
    (*Build->config)[Config::h][__STRING(MODE_FLIES)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_GAMEOFLIFE) && !(MODE_GAMEOFLIFE)
    (*Build->config)[Config::h][__STRING(MODE_GAMEOFLIFE)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_GAMEOFLIFECLOCK) && !(MODE_GAMEOFLIFECLOCK)
    (*Build->config)[Config::h][__STRING(MODE_GAMEOFLIFECLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_GLITTER) && !(MODE_GLITTER)
    (*Build->config)[Config::h][__STRING(MODE_GLITTER)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_HOMEASSISTANTWEATHER) && !(MODE_HOMEASSISTANTWEATHER)
    (*Build->config)[Config::h][__STRING(MODE_HOMEASSISTANTWEATHER)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_HOMETHERMOMETER) && !(MODE_HOMETHERMOMETER)
    (*Build->config)[Config::h][__STRING(MODE_HOMETHERMOMETER)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_JAGGEDWAVEFORM) && !(MODE_JAGGEDWAVEFORM)
    (*Build->config)[Config::h][__STRING(MODE_JAGGEDWAVEFORM)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_LEAFFALL) && !(MODE_LEAFFALL)
    (*Build->config)[Config::h][__STRING(MODE_LEAFFALL)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_LINES) && !(MODE_LINES)
    (*Build->config)[Config::h][__STRING(MODE_LINES)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_NOISE) && !(MODE_NOISE)
    (*Build->config)[Config::h][__STRING(MODE_NOISE)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_OPENMETRO) && !(MODE_OPENMETRO)
    (*Build->config)[Config::h][__STRING(MODE_OPENMETRO)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_OPENWEATHER) && !(MODE_OPENWEATHER)
    (*Build->config)[Config::h][__STRING(MODE_OPENWEATHER)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_PINGPONG) && !(MODE_PINGPONG)
    (*Build->config)[Config::h][__STRING(MODE_PINGPONG)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_PINGPONGCLOCK) && !(MODE_PINGPONGCLOCK)
    (*Build->config)[Config::h][__STRING(MODE_PINGPONGCLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_PIXELSEQUENCE) && !(MODE_PIXELSEQUENCE)
    (*Build->config)[Config::h][__STRING(MODE_PIXELSEQUENCE)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_RAIN) && !(MODE_RAIN)
    (*Build->config)[Config::h][__STRING(MODE_RAIN)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_RING) && !(MODE_RING)
    (*Build->config)[Config::h][__STRING(MODE_RING)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_SMALLCLOCK) && !(MODE_SMALLCLOCK)
    (*Build->config)[Config::h][__STRING(MODE_SMALLCLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_SMOOTHWAVEFORM) && !(MODE_SMOOTHWAVEFORM)
    (*Build->config)[Config::h][__STRING(MODE_SMOOTHWAVEFORM)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_SNAKE) && !(MODE_SNAKE)
    (*Build->config)[Config::h][__STRING(MODE_SNAKE)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_SNAKECLOCK) && !(MODE_SNAKECLOCK)
    (*Build->config)[Config::h][__STRING(MODE_SNAKECLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_STARS) && !(MODE_STARS)
    (*Build->config)[Config::h][__STRING(MODE_STARS)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_TICKER) && !(MODE_TICKER)
    (*Build->config)[Config::h][__STRING(MODE_TICKER)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_TICKINGCLOCK) && !(MODE_TICKINGCLOCK)
    (*Build->config)[Config::h][__STRING(MODE_TICKINGCLOCK)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_WAVEFORM) && !(MODE_WAVEFORM)
    (*Build->config)[Config::h][__STRING(MODE_WAVEFORM)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_WORLDWEATHERONLINE) && !(MODE_WORLDWEATHERONLINE)
    (*Build->config)[Config::h][__STRING(MODE_WORLDWEATHERONLINE)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_WTTRIN) && !(MODE_WTTRIN)
    (*Build->config)[Config::h][__STRING(MODE_WTTRIN)] = false;
#endif
#if EXTENSION_BUILD && defined(MODE_YR) && !(MODE_YR)
    (*Build->config)[Config::h][__STRING(MODE_YR)] = false;
#endif

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_mode");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"mode\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::icon] = "mdi:format-list-bulleted";
        component[Abbreviations::name] = "Mode";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[Abbreviations::options].to<JsonArray>();
        for (const ModeModule *mode : modules)
        {
            options.add(mode->name);
        }
        component[Abbreviations::platform] = "select";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.mode}}";
    }
#ifdef F_VERBOSE
    {
        const std::string id = std::string(name).append("_stack");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "data_size";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::icon] = "mdi:memory";
        component[Abbreviations::name] = std::string(name).append(" stack");
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "kB";
        component[Abbreviations::value_template] = "{{value_json.stack/2**10}}";
    }
#endif // F_VERBOSE
#endif // EXTENSION_HOMEASSISTANT

    for (ModeModule *mode : modules)
    {
        mode->setup();
    }
#ifdef F_VERBOSE
    Serial.printf("%s: setup complete\n", name);
#endif
}

void ModesService::ready()
{
    xTaskCreate(&onTask, name, stackSize, nullptr, 2, &taskHandle);
    if (!active)
    {
        switch (esp_reset_reason())
        {
        case esp_reset_reason_t::ESP_RST_BROWNOUT:
        case esp_reset_reason_t::ESP_RST_INT_WDT:
        case esp_reset_reason_t::ESP_RST_PANIC:
        case esp_reset_reason_t::ESP_RST_TASK_WDT:
        case esp_reset_reason_t::ESP_RST_WDT:
            break;
        default:
            Preferences Storage;
            Storage.begin(name, true);
            if (Storage.isKey("active"))
            {
                const String _active = Storage.getString("active");
                Storage.end();
                set(_active.c_str());
            }
            else
            {
                Storage.end();
            }
        }
        if (!active)
        {
            set(modules[random(0, modules.size())]->name);
        }
    }
}

void ModesService::handle()
{
    if (pending && millis() - lastMillis > 2048)
    {
        if (active)
        {
            active->wake();
            set(true, name);
        }
        pending = false;
    }
#ifdef F_VERBOSE
    else if (active && Display.getPower() && millis() - _lastMillis > UINT16_MAX)
    {
        transmit();
    }
#endif // F_VERBOSE
}

void ModesService::set(bool enable, const char *const source)
{
    if (enable && taskHandle && eTaskGetState(taskHandle) == eTaskState::eSuspended)
    {
#ifdef F_DEBUG
        Serial.printf("%s: resuming mode\n", source);
#endif
        vTaskResume(taskHandle);
    }
    else if (!enable && taskHandle && eTaskGetState(taskHandle) != eTaskState::eSuspended)
    {
#ifdef F_DEBUG
        Serial.printf("%s: suspending mode\n", source);
#endif
        vTaskSuspend(taskHandle);
    }
}

void ModesService::set(const char *const name)
{
    if (active && !strcmp(active->name, name))
    {
        return;
    }
    for (ModeModule *mode : modules)
    {
        if (!strcmp(mode->name, name))
        {
            set(mode);
            return;
        }
    }
}

void ModesService::set(ModeModule *mode)
{
    set(false, name);
    teardown();
    active = mode;
#ifdef F_INFO
    Serial.printf("%s: %s\n", name, active->name);
#endif

    Preferences Storage;
    Storage.begin(name);
    Storage.putString("active", active->name);
    Storage.end();

    splash();
    lastMillis = millis();
    pending = true;
    transmit();
}

void ModesService::splash()
{
    if (!Display.getPower())
    {
#ifdef F_INFO
        Serial.printf("%s: power\n", name);
#endif
        Display.setPower(true);
    }
    const String _name = active->name;
    std::vector<String> chunks = {""};
    uint8_t line = 0;
    for (uint8_t i = 0; i < _name.length(); ++i)
    {
        switch (_name[i])
        {
        case 0x20: //   Space
        case 0x2D: // - Hyphen-minus
            chunks.push_back("");
            ++line;
            break;
        default:
            chunks[line] += _name[i];
        }
    }
    uint8_t height = 0;
    std::vector<TextHandler> texts;
    for (const String &chunk : chunks)
    {
        texts.push_back(TextHandler(chunk, FontMicro));
        height += texts.back().getHeight();
        if (height >= ROWS)
        {
            break;
        }
    }
    const uint8_t margin = (ROWS - min<uint8_t>(ROWS, height)) / (chunks.size() + 1);
    uint8_t y = margin;
    Display.clear();
    for (TextHandler &text : texts)
    {
        text.draw((COLUMNS - min<uint8_t>(COLUMNS, text.getWidth())) / 2, y);
        y += text.getHeight() + max<uint8_t>(1, margin);
    }
    Display.flush();
}

void ModesService::teardown()
{
    if (active)
    {
        active->sleep();
    }
    Display.clear();
}

const std::vector<ModeModule *> &ModesService::getAll() const
{
    return modules;
}

void ModesService::next()
{
    if (active)
    {
        const char *const _name = active->name;
        std::vector<ModeModule *>::const_iterator _modules = std::find_if(modules.begin(), modules.end(), [_name](const ModeModule *_mode)
                                                                          { return !strcmp(_mode->name, _name); });
        if (_modules == modules.end())
        {
            set((*modules.begin())->name);
        }
        else if (!Display.getPower())
        {
            Display.setPower(true);
        }
        else
        {
            ++_modules;
            if (_modules == modules.end())
            {
                set((*modules.begin())->name);
                Display.setPower(false);
            }
            else
            {
                set((*_modules)->name);
            }
        }
    }
    else
    {
        set((*modules.begin())->name);
    }
}

void ModesService::previous()
{
    if (active)
    {
        const char *const _name = active->name;
        std::vector<ModeModule *>::const_iterator _modules = std::find_if(modules.begin(), modules.end(), [_name](const ModeModule *_mode)
                                                                          { return !strcmp(_mode->name, _name); });
        if (_modules == modules.begin())
        {
            set((*(modules.end() - 1))->name);
            Display.setPower(false);
        }
        else if (!Display.getPower())
        {
            Display.setPower(true);
        }
        else
        {
            set((*(_modules - 1))->name);
        }
    }
    else
    {
        set((*(modules.end() - 1))->name);
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
#ifdef F_VERBOSE
    doc["stack"] = stackSize - uxTaskGetStackHighWaterMark(taskHandle);
    _lastMillis = millis();
#endif
    Device.transmit(doc, name);
}

void ModesService::receiverHook(const JsonDocument doc)
{
    // Mode
    if (doc["mode"].is<const char *>())
    {
        set(doc["mode"].as<const char *>());
    }
}

void ModesService::onTask(void *parameter)
{
    for (;;)
    {
        if (Modes.active && Display.getPower())
        {
            Modes.active->handle();
            Display.flush();
        }
        vTaskDelay(1);
    }
}

ModesService &ModesService::getInstance()
{
    static ModesService instance;
    return instance;
}

ModesService &Modes = Modes.getInstance();
