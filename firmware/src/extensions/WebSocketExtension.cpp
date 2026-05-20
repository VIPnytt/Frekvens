#if EXTENSION_WEBSOCKET

#include "extensions/WebSocketExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

void WebSocketExtension::begin()
{
    server->onEvent(&onEvent);
    WebServer.http->addHandler(server);
}

void WebSocketExtension::handle() { server->cleanupClients(); }

void WebSocketExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc[source].set(payload);
    const size_t length{measureJson(doc)};
    std::vector<char> message(length + 1U);
    serializeJson(doc, message.data(), length + 1U);
    server->textAll(message.data(), length);
}

void WebSocketExtension::onEvent(AsyncWebSocket *server, // NOLINT(misc-unused-parameters)
                                 AsyncWebSocketClient *client, AwsEventType type, void *arg, const uint8_t *data,
                                 size_t len)
{
    switch (type)
    {
    case AwsEventType::WS_EVT_CONNECT:
    {
        const JsonObjectConst transmits{Device.getTransmits()};
        const size_t length{measureJson(transmits)};
        std::vector<char> message(length + 1U);
        serializeJson(transmits, message.data(), length + 1U);
        client->text(message.data(), length);
    }
    break;
    case AwsEventType::WS_EVT_DATA:
    {
        const AwsFrameInfo *info{static_cast<AwsFrameInfo *>(arg)};
        if (info->message_opcode == AwsFrameType::WS_TEXT)
        {
            if (!info->final || info->index != 0U || info->index + len != info->len)
            {
                if (info->num == 0U && info->index == 0U)
                {
                    buffer.clear();
                }
                buffer.insert(buffer.end(), data, data + len);
                if (!info->final || info->index + len != info->len)
                {
                    return;
                }
                data = buffer.data();
                len = buffer.size();
            }
            JsonDocument doc; // NOLINT(misc-const-correctness)
            if (deserializeJson(doc, data, len) == DeserializationError::Code::Ok && doc.is<JsonObjectConst>())
            {
                for (const JsonPairConst pair : doc.as<JsonObjectConst>())
                {
                    if (pair.value().is<JsonObjectConst>())
                    {
                        Device.receive(pair.value().as<JsonObjectConst>(),
                                       name,
                                       std::string_view(pair.key().c_str(), pair.key().size()));
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
