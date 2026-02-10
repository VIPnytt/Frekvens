#pragma once

#if EXTENSION_WEBSOCKET

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class WebSocketExtension final : public ExtensionModule
{
private:
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                        const uint8_t *data, size_t len);

public:
    explicit WebSocketExtension();

    AsyncWebSocket *server = new AsyncWebSocket("/websocket");

    void begin() override;
    void handle() override;

    void onTransmit(JsonObjectConst payload, const char *source) override;
};

extern WebSocketExtension *WebSocket;

#endif // EXTENSION_WEBSOCKET
