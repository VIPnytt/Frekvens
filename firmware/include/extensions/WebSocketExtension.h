#pragma once

#if EXTENSION_WEBSOCKET

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class WebSocketExtension : public ExtensionModule
{
private:
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

public:
    WebSocketExtension();

    AsyncWebSocket *server = new AsyncWebSocket("/websocket");

    void begin() override;
    void handle() override;

    void onTransmit(const JsonDocument &doc, const char *const source) override;
};

extern WebSocketExtension *WebSocket;

#endif // EXTENSION_WEBSOCKET
