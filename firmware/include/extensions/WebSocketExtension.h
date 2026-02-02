#pragma once

#if EXTENSION_WEBSOCKET

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class WebSocketExtension final : public ExtensionModule
{
private:
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                        const uint8_t *data, size_t len);

protected:
    ~WebSocketExtension() = default;

public:
    WebSocketExtension();
    WebSocketExtension(const WebSocketExtension &) = delete;
    WebSocketExtension &operator=(const WebSocketExtension &) = delete;

    AsyncWebSocket *server = new AsyncWebSocket("/websocket");

    void begin() override;
    void handle() override;

    void onTransmit(const JsonDocument &doc, const char *source) override;
};

extern WebSocketExtension *WebSocket;

#endif // EXTENSION_WEBSOCKET
