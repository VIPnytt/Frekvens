#if EXTENSION_SERVERSENTEVENTS

#include "extensions/ServerSentEventsExtension.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/WebServerService.h"

#include <HTTPClient.h>

ServerSentEventsExtension *ServerSentEvents = nullptr;

ServerSentEventsExtension::ServerSentEventsExtension() : ExtensionModule("Server-Sent Events")
{
    ServerSentEvents = this;
}

void ServerSentEventsExtension::begin()
{
    client->onConnect(&onConnect);
    WebServer.http->addHandler(client);
}

void ServerSentEventsExtension::onTransmit(const JsonDocument &doc, const char *const source)
{
    const size_t length = measureJson(doc);
    std::vector<char> payload(length + 1);
    serializeJson(doc, payload.data(), length + 1);
    client->send(payload.data(), source);
}

void ServerSentEventsExtension::onConnect(AsyncEventSourceClient *client)
{
    JsonDocument doc = Device.getTransmits();
    for (const JsonPairConst &pair : doc.as<JsonObjectConst>())
    {
        const size_t length = measureJson(pair.value());
        std::vector<char> payload(length + 1);
        serializeJson(pair.value(), payload.data(), length + 1);
        client->send(payload.data(), pair.key().c_str());
    }
}

#endif // EXTENSION_SERVERSENTEVENTS
