#include "config/constants.h"

#if EXTENSION_WEBSOCKET

#include "extensions/WebSocketExtension.h"
#include "services/DeviceService.h"
#include "services/WebServerService.h"

WebSocketExtension *WebSocket = nullptr;

WebSocketExtension::WebSocketExtension() : ExtensionModule("WebSocket")
{
    WebSocket = this;
}

void WebSocketExtension::ready()
{
    server->onEvent(&onEvent);
    WebServer.http->addHandler(server);
}

void WebSocketExtension::handle()
{
    if (pending)
    {
        pending = false;
        const JsonDocument doc = Device.getTransmits();
        const size_t length = measureJson(doc);
        char *payload = new char[length + 1];
        serializeJson(doc, payload, length + 1);
        server->textAll(payload);
        delete[] payload;
    }
    else
    {
        server->cleanupClients();
    }
}

void WebSocketExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    JsonDocument _doc;
    _doc[source] = doc;
    const size_t length = measureJson(_doc);
    char *payload = new char[length + 1];
    serializeJson(_doc, payload, length + 1);
    server->textAll(payload);
    delete[] payload;
}

void WebSocketExtension::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (type == AwsEventType::WS_EVT_DATA && info->opcode == AwsFrameType::WS_TEXT)
    {
#ifdef F_DEBUG
        if (len < info->len)
        {
            Serial.print(WebSocket->name);
            Serial.println(": chunked messages is currently not supported");
            return;
        }
#endif
        JsonDocument doc;
        if (!deserializeJson(doc, data, len) && doc.is<JsonObjectConst>())
        {
            for (const JsonPairConst pair : doc.as<JsonObjectConst>())
            {
                if (pair.value().is<JsonObjectConst>())
                {
                    Device.receive(pair.value(), WebSocket->name, pair.key().c_str());
                }
            }
        }
    }
    else if (type == AwsEventType::WS_EVT_CONNECT)
    {
        WebSocket->pending = true;
    }
}

#endif // EXTENSION_WEBSOCKET
