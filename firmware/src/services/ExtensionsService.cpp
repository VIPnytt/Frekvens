#include "services/ExtensionsService.h"

#include "services/DeviceService.h"

void ExtensionsService::configure()
{
    for (ExtensionModule *extension : modules)
    {
        extension->configure();
    }
}

void ExtensionsService::begin()
{
    for (ExtensionModule *extension : modules)
    {
        extension->begin();
    }
    xTaskCreate(&onTask, "Extensions", stackSize, nullptr, 1, &taskHandle);
    transmit();
}

std::span<ExtensionModule *const> ExtensionsService::getAll() { return modules; }

void ExtensionsService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray list{doc["list"].to<JsonArray>()};
    for (const ExtensionModule *extension : modules)
    {
        list.add(extension->name);
    }
    lastMillis = millis();
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ExtensionsService::onTask(void *parameter) // NOLINT(misc-unused-parameters)
{
    for (;;)
    {
        for (ExtensionModule *extension : Extensions.getAll())
        {
            extension->handle();
        }
        vTaskDelay(1);
    }
}

#if EXTENSION_HOMEASSISTANT
HomeAssistantExtension &ExtensionsService::HomeAssistant() { return extensionHomeAssistant; }
#endif
#if EXTENSION_MICROPHONE
MicrophoneExtension &ExtensionsService::Microphone() { return extensionMicrophone; }
#endif
#if EXTENSION_MQTT
MqttExtension &ExtensionsService::MQTT() { return extensionMqtt; }
#endif
#if EXTENSION_PHOTOCELL
PhotocellExtension &ExtensionsService::Photocell() { return extensionPhotocell; }
#endif
#if EXTENSION_PLAYLIST
PlaylistExtension &ExtensionsService::Playlist() { return extensionPlaylist; }
#endif
#if EXTENSION_SERVERSENTEVENTS
ServerSentEventsExtension &ExtensionsService::ServerSentEvents() { return extensionServerSentEvents; }
#endif
#if EXTENSION_STATUSLED
StatusLedExtension &ExtensionsService::StatusLed() { return extensionStatusLed; }
#endif
#if EXTENSION_WEBSOCKET
WebSocketExtension &ExtensionsService::WebSocket() { return extensionWebSocket; }
#endif

ExtensionsService &ExtensionsService::getInstance()
{
    static ExtensionsService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
ExtensionsService &Extensions = ExtensionsService::getInstance();
