#if EXTENSION_SERVERSENTEVENTS

#include "extensions/ServerSentEventsExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

/**
 * @brief Registers the Server-Sent Events connection callback and attaches the event source to the web server.
 */
void ServerSentEventsExtension::begin()
{
    events.onConnect(&onConnect);
    WebServer.http.addHandler(&events);
}

/**
 * @brief Sends a JSON payload through the Server-Sent Events stream.
 *
 * @param payload JSON payload to transmit.
 * @param source Event name associated with the payload.
 */
void ServerSentEventsExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    const size_t length{measureJson(payload)};
    std::vector<char> message(length + 1U);
    serializeJson(payload, message.data(), length + 1U);
    events.send(message.data(), source.data());
}

/**
 * @brief Sends the current transmit values to a newly connected client.
 *
 * @param client Client that receives the transmit events.
 */
void ServerSentEventsExtension::onConnect(AsyncEventSourceClient *client)
{
    for (const JsonPairConst pair : Device.getTransmits())
    {
        const size_t length{measureJson(pair.value())};
        std::vector<char> message(length + 1U);
        serializeJson(pair.value(), message.data(), length + 1U);
        client->send(message.data(), pair.key().c_str());
    }
}

#endif // EXTENSION_SERVERSENTEVENTS
