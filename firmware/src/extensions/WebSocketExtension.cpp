#if EXTENSION_WEBSOCKET

#include "extensions/WebSocketExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

WebSocketExtension *WebSocket = nullptr;

WebSocketExtension::WebSocketExtension() : ExtensionModule("WebSocket") { WebSocket = this; }

void WebSocketExtension::begin()
{
    server->onEvent(&onEvent);
    WebServer.http->addHandler(server);
}

void WebSocketExtension::handle() { server->cleanupClients(); }

void WebSocketExtension::onTransmit(JsonObjectConst payload, const char *source)
{
    JsonDocument doc;
    doc[source].set(payload);
    const size_t length = measureJson(doc);
    std::vector<char> message(length + 1);
    serializeJson(doc, message.data(), length + 1);
    server->textAll(message.data(), length);
}

void WebSocketExtension::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                                 const uint8_t *data, size_t len)
{
    switch (type)
    {
    case AwsEventType::WS_EVT_CONNECT:
    {
        const JsonObjectConst transmits = Device.getTransmits();
        const size_t length = measureJson(transmits);
        std::vector<char> message(length + 1);
        serializeJson(transmits, message.data(), length + 1);
        client->text(message.data(), length);
    }
    break;
    case AwsEventType::WS_EVT_DATA:
    {
        if (static_cast<AwsFrameInfo *>(arg)->opcode == AwsFrameType::WS_TEXT)
        {
            JsonDocument doc;
            if (!deserializeJson(doc, data, len) && doc.is<JsonObjectConst>())
            {
                for (const JsonPairConst pair : doc.as<JsonObjectConst>())
                {
                    if (pair.value().is<JsonObjectConst>())
                    {
                        Device.receive(pair.value().as<JsonObjectConst>(), WebSocket->name, pair.key().c_str());
                    }
                }
            }
        }
    }
    break;
    default:
        break;
    }
}

#endif // EXTENSION_WEBSOCKET
