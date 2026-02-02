#pragma once

#if EXTENSION_SERVERSENTEVENTS

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class ServerSentEventsExtension final : public ExtensionModule
{
protected:
    ~ServerSentEventsExtension() = default;

public:
    ServerSentEventsExtension();
    ServerSentEventsExtension(const ServerSentEventsExtension &) = delete;
    ServerSentEventsExtension &operator=(const ServerSentEventsExtension &) = delete;

    AsyncEventSource *client = new AsyncEventSource("/server-sent%20events");

    void begin() override;

    void onTransmit(const JsonDocument &doc, const char *source) override;

    static void onConnect(AsyncEventSourceClient *client);
};

extern ServerSentEventsExtension *ServerSentEvents;

#endif // EXTENSION_SERVERSENTEVENTS
