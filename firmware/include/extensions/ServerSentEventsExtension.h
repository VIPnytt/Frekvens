#pragma once

#if EXTENSION_SERVERSENTEVENTS

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class ServerSentEventsExtension final : public ExtensionModule
{
public:
    static constexpr std::string_view name{"Server-Sent Events"};

    explicit ServerSentEventsExtension() : ExtensionModule(name) {};

    AsyncEventSource events{"/server-sent%20events"};

    void begin() override;

    void onTransmit(JsonObjectConst payload, std::string_view source) override;

    static void onConnect(AsyncEventSourceClient *client);
};

#endif // EXTENSION_SERVERSENTEVENTS
