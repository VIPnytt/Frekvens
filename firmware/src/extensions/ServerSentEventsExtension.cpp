#if EXTENSION_SERVERSENTEVENTS

#include "extensions/ServerSentEventsExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

void ServerSentEventsExtension::begin()
{
    client->onConnect(&onConnect);
    WebServer.http->addHandler(client);
}

void ServerSentEventsExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    const size_t length = measureJson(payload);
    std::vector<char> message(length + 1);
    serializeJson(payload, message.data(), length + 1);
    client->send(message.data(), source.data());
}

void ServerSentEventsExtension::onConnect(AsyncEventSourceClient *client)
{
    for (const JsonPairConst pair : Device.getTransmits())
    {
        const size_t length = measureJson(pair.value());
        std::vector<char> message(length + 1);
        serializeJson(pair.value(), message.data(), length + 1);
        client->send(message.data(), pair.key().c_str());
    }
}

#endif // EXTENSION_SERVERSENTEVENTS
