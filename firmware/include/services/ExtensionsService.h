#pragma once

#include <vector>

#include "config/constants.h"
#include "extensions/AlexaExtension.h"
#include "extensions/BuildExtension.h"
#include "extensions/ButtonExtension.h"

#include "extensions/HomeAssistantExtension.h"
#include "extensions/InfraredExtension.h"
#include "extensions/MessageExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "extensions/MqttExtension.h"
#include "extensions/OtaExtension.h"
#include "extensions/PhotocellExtension.h"
#include "extensions/PlaylistExtension.h"
#include "extensions/RestfulExtension.h"
#include "extensions/RtcExtension.h"
#include "extensions/ServerSentEventsExtension.h"
#include "extensions/SignalExtension.h"
#include "extensions/WebAppExtension.h"
#include "extensions/WebSocketExtension.h"
#include "modules/ExtensionModule.h"
#include "modules/ServiceModule.h"

class ExtensionsService : public ServiceModule
{
private:
    ExtensionsService() : ServiceModule("Extensions") {};

    const std::vector<ExtensionModule *> modules = {
#if EXTENSION_ALEXA
        new AlexaExtension(),
#endif
#if EXTENSION_BUILD
        new BuildExtension(),
#endif
#if EXTENSION_BUTTON
        new ButtonExtension(),
#endif
#if EXTENSION_HOMEASSISTANT
        new HomeAssistantExtension(),
#endif
#if EXTENSION_INFRARED
        new InfraredExtension(),
#endif
#if EXTENSION_MESSAGE
        new MessageExtension(),
#endif
#if EXTENSION_MICROPHONE
        new MicrophoneExtension(),
#endif
#if EXTENSION_MQTT
        new MqttExtension(),
#endif
#if EXTENSION_OTA
        new OtaExtension(),
#endif
#if EXTENSION_PHOTOCELL
        new PhotocellExtension(),
#endif
#if EXTENSION_PLAYLIST
        new PlaylistExtension(),
#endif
#if EXTENSION_RESTFUL
        new RestfulExtension(),
#endif
#if EXTENSION_RTC
        new RtcExtension(),
#endif
#if EXTENSION_SERVERSENTEVENTS
        new ServerSentEventsExtension(),
#endif
#if EXTENSION_SIGNAL
        new SignalExtension(),
#endif
#if EXTENSION_WEBAPP
        new WebAppExtension(),
#endif
#if EXTENSION_WEBSOCKET
        new WebSocketExtension(),
#endif
    };

    unsigned long lastMillis = 0;

#ifdef TASK_STACK_EXTENSIONS
    static constexpr uint16_t stackSize = TASK_STACK_EXTENSIONS;
#else
    static constexpr uint16_t stackSize = 1 << 12; // 4 kB
#endif // TASK_STACK_EXTENSIONS

    void transmit();

    static void onTask(void *parameter = nullptr);

public:
    TaskHandle_t taskHandle = nullptr;

    void setup();
    void ready();

#ifdef F_VERBOSE
    void handle();
#endif

    const std::vector<ExtensionModule *> &getAll() const;

    static ExtensionsService &getInstance();
};

extern ExtensionsService &Extensions;
