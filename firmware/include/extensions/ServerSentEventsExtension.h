#pragma once

#include "config/constants.h"

#if EXTENSION_SERVERSENTEVENTS

#include <ESPAsyncWebServer.h>
#include <fauxmoESP.h>

#include "modules/ExtensionModule.h"

class ServerSentEventsExtension : public ExtensionModule
{
public:
    ServerSentEventsExtension();

    AsyncEventSource *sse = new AsyncEventSource("/api/events");

    void ready() override;

    void transmitterHook(const JsonDocument &doc, const char *const source) override;

    static void onConnect(AsyncEventSourceClient *client);
};

extern ServerSentEventsExtension *ServerSentEvents;

#endif // EXTENSION_SERVERSENTEVENTS
