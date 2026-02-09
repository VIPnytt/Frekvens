#pragma once

#if EXTENSION_SERVERSENTEVENTS

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class ServerSentEventsExtension final : public ExtensionModule
{
public:
    explicit ServerSentEventsExtension();

    AsyncEventSource *client = new AsyncEventSource("/server-sent%20events");

    void begin() override;

    void onTransmit(JsonObjectConst payload, const char *source) override;

    static void onConnect(AsyncEventSourceClient *client);
};

extern ServerSentEventsExtension *ServerSentEvents;

#endif // EXTENSION_SERVERSENTEVENTS
