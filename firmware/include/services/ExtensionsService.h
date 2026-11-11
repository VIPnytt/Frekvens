#pragma once

#include "extensions/AlexaExtension.h"
#include "extensions/ButtonExtension.h"
#include "extensions/InfraredExtension.h"
#include "extensions/MessageExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "extensions/MqttExtension.h"
#include "extensions/OtaExtension.h"
#include "extensions/PhotocellExtension.h"
#include "extensions/PlaylistExtension.h"
#include "extensions/RestfulExtension.h"
#include "extensions/RtcExtension.h"
#include "extensions/ScreenshotExtension.h"
#include "extensions/SignalExtension.h"
#include "extensions/WebAppExtension.h"
#include "extensions/WebSocketExtension.h"
#include "modules/ServiceModule.h"

#include <array>
#include <span>

class ExtensionsService final : public ServiceModule
{
private:
    explicit ExtensionsService() : ServiceModule("Extensions") {};

    unsigned long lastMillis{0UL};

#if EXTENSION_ALEXA
    AlexaExtension extensionAlexa;
#endif
#if EXTENSION_BUTTON
    ButtonExtension extensionButton;
#endif
#if EXTENSION_INFRARED
    InfraredExtension extensionInfrared;
#endif
#if EXTENSION_MESSAGE
    MessageExtension extensionMessage;
#endif
#if EXTENSION_MICROPHONE
    MicrophoneExtension extensionMicrophone;
#endif
#if EXTENSION_MQTT
    MqttExtension extensionMqtt;
#endif
#if EXTENSION_OTA
    OtaExtension extensionOta;
#endif
#if EXTENSION_PHOTOCELL
    PhotocellExtension extensionPhotocell;
#endif
#if EXTENSION_PLAYLIST
    PlaylistExtension extensionPlaylist;
#endif
#if EXTENSION_RESTFUL
    RestfulExtension extensionRestful;
#endif
#if EXTENSION_RTC
    RtcExtension extensionRtc;
#endif
#if EXTENSION_SCREENSHOT
    ScreenshotExtension extensionScreenshot;
#endif
#if EXTENSION_SIGNAL
    SignalExtension extensionSignal;
#endif
    WebAppExtension extensionWebApp;

    WebSocketExtension extensionWebSocket;

    const std::array<ExtensionModule *, 2U + COUNT_EXTENSION> modules{
#if EXTENSION_ALEXA
        &extensionAlexa,
#endif
#if EXTENSION_BUTTON
        &extensionButton,
#endif
#if EXTENSION_INFRARED
        &extensionInfrared,
#endif
#if EXTENSION_MESSAGE
        &extensionMessage,
#endif
#if EXTENSION_MICROPHONE
        &extensionMicrophone,
#endif
#if EXTENSION_MQTT
        &extensionMqtt,
#endif
#if EXTENSION_OTA
        &extensionOta,
#endif
#if EXTENSION_PHOTOCELL
        &extensionPhotocell,
#endif
#if EXTENSION_PLAYLIST
        &extensionPlaylist,
#endif
#if EXTENSION_RESTFUL
        &extensionRestful,
#endif
#if EXTENSION_RTC
        &extensionRtc,
#endif
#if EXTENSION_SCREENSHOT
        &extensionScreenshot,
#endif
#if EXTENSION_SIGNAL
        &extensionSignal,
#endif
        &extensionWebApp,
        &extensionWebSocket,
    };

    void transmit();

    static void onTask(void *parameter = nullptr);

public:
    static constexpr uint16_t stackSize = 1U << 13U; // 8 kB

    TaskHandle_t taskHandle = nullptr;

    void configure();
    void begin();

#if EXTENSION_MICROPHONE
    MicrophoneExtension &Microphone();
#endif
#if EXTENSION_MQTT
    MqttExtension &MQTT();
#endif
#if EXTENSION_PHOTOCELL
    PhotocellExtension &Photocell();
#endif
#if EXTENSION_PLAYLIST
    PlaylistExtension &Playlist();
#endif
    WebSocketExtension &WebSocket();

    [[nodiscard]] std::span<ExtensionModule *const> getAll();

    static ExtensionsService &getInstance();
};

extern ExtensionsService &Extensions; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
