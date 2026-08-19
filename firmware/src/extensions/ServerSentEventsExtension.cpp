#if EXTENSION_SERVERSENTEVENTS

#include "extensions/ServerSentEventsExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

void ServerSentEventsExtension::begin()
{
    events.onConnect(&onConnect);
    WebServer.http.addHandler(&events);
}

void ServerSentEventsExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    const size_t length{measureJson(payload)};
    std::vector<char> message(length + 1U);
    serializeJson(payload, message.data(), length + 1U);
    events.send(message.data(), source.data());
}

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
