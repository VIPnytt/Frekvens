#pragma once

#if EXTENSION_WEBSOCKET

#include "modules/ExtensionModule.h"

#include <AsyncWebSocket.h>

class WebSocketExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"WebSocket"};

    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                        const uint8_t *data, size_t len);

public:
    explicit WebSocketExtension() : ExtensionModule(name) {};

    AsyncWebSocket *server = new AsyncWebSocket("/websocket");

    void begin() override;
    void handle() override;

    void onTransmit(JsonObjectConst payload, std::string_view source) override;
};

#endif // EXTENSION_WEBSOCKET
