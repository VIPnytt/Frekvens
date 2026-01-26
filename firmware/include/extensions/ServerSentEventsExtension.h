#pragma once

#if EXTENSION_SERVERSENTEVENTS

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class ServerSentEventsExtension : public ExtensionModule
{
public:
    ServerSentEventsExtension();

    AsyncEventSource *client = new AsyncEventSource("/server-sent%20events");

    void begin() override;

    void onTransmit(const JsonDocument &doc, const char *const source) override;

    static void onConnect(AsyncEventSourceClient *client);
};

extern ServerSentEventsExtension *ServerSentEvents;

#endif // EXTENSION_SERVERSENTEVENTS
