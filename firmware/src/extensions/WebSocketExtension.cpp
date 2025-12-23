#if EXTENSION_WEBSOCKET

#include "extensions/WebSocketExtension.h"
#include "services/DeviceService.h"
#include "services/WebServerService.h"

WebSocketExtension *WebSocket = nullptr;

WebSocketExtension::WebSocketExtension() : ExtensionModule("WebSocket")
{
    WebSocket = this;
}

void WebSocketExtension::begin()
{
    server->onEvent(&onEvent);
    WebServer.http->addHandler(server);
}

void WebSocketExtension::handle()
{
    server->cleanupClients();
}

void WebSocketExtension::onTransmit(const JsonDocument &doc, const char *const source)
{
    JsonDocument _doc;
    _doc[source] = doc;
    const size_t length = measureJson(_doc);
    std::vector<char> payload(length + 1);
    serializeJson(_doc, payload.data(), length + 1);
    server->textAll(payload.data(), length);
}

void WebSocketExtension::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case AwsEventType::WS_EVT_CONNECT:
    {
        const JsonDocument doc = Device.getTransmits();
        const size_t length = measureJson(doc);
        std::vector<char> payload(length + 1);
        serializeJson(doc, payload.data(), length + 1);
        client->text(payload.data(), length);
    }
    break;
    case AwsEventType::WS_EVT_DATA:
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->opcode == AwsFrameType::WS_TEXT)
        {
            JsonDocument doc;
            if (!deserializeJson(doc, data, len) && doc.is<JsonObjectConst>())
            {
                for (const JsonPairConst &pair : doc.as<JsonObjectConst>())
                {
                    if (pair.value().is<JsonObjectConst>())
                    {
                        Device.receive(pair.value(), WebSocket->name, pair.key().c_str());
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
