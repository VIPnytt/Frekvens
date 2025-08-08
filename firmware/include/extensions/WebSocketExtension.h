#pragma once

#include "config/constants.h"

#if EXTENSION_WEBSOCKET

#include <ESPAsyncWebServer.h>

#include "modules/ExtensionModule.h"

class WebSocketExtension : public ExtensionModule
{
private:
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

public:
    WebSocketExtension();

    AsyncWebSocket *ws = new AsyncWebSocket("/ws");

    void ready() override;
    void handle() override;

    void transmitterHook(const JsonDocument &doc, const char *const source) override;
};

extern WebSocketExtension *WebSocket;

#endif // EXTENSION_WEBSOCKET
