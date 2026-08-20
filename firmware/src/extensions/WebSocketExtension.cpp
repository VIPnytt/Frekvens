#if EXTENSION_WEBSOCKET

#include "extensions/WebSocketExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

#include <span>

/**
 * @brief Registers the WebSocket event callback and attaches the server to the HTTP server.
 */
void WebSocketExtension::begin()
{
    server->onEvent(&onEvent);
    WebServer.http.addHandler(server);
}

/**
 * @brief Removes inactive WebSocket clients from the server.
 */
void WebSocketExtension::handle() { server->cleanupClients(); }

/**
 * @brief Broadcasts a payload to all connected WebSocket clients.
 *
 * @param payload JSON payload to transmit.
 * @param source Property name under which the payload is sent.
 */
void WebSocketExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    JsonDocument doc{};
    doc[source].set(payload);
    const size_t length{measureJson(doc)};
    std::vector<char> message(length + 1U);
    serializeJson(doc, message.data(), length + 1U);
    server->textAll(message.data(), length);
}

/**
 * @brief Handles WebSocket connections and text messages.
 *
 * Sends available transmits to newly connected clients and forwards each
 * object-valued property in a valid JSON object to the device. Reassembles
 * fragmented text messages before processing them.
 *
 * @param client WebSocket client associated with the event.
 * @param data Event payload data.
 * @param len Number of bytes in the event payload.
 */
void WebSocketExtension::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                                 const uint8_t *data, size_t len)
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
            if (info->final == 0U || info->index != 0U || info->index + len != info->len)
            {
                if (info->num == 0U && info->index == 0U)
                {
                    buffer.clear();
                }
                const std::span<const uint8_t> chunk{data, len};
                buffer.insert(buffer.end(), chunk.begin(), chunk.end());
                if (info->final == 0U || info->index + len != info->len)
                {
                    return;
                }
                data = buffer.data();
                len = buffer.size();
            }
            JsonDocument doc{};
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
