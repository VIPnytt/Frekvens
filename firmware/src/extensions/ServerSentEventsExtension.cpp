#include "config/constants.h"

#if EXTENSION_SERVERSENTEVENTS

#include <HTTPClient.h>

#include "extensions/ServerSentEventsExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/WebServerService.h"

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
    char *payload = new char[length + 1];
    serializeJson(doc, payload, length + 1);
    client->send(payload, source);
    delete[] payload;
}

void ServerSentEventsExtension::onConnect(AsyncEventSourceClient *client)
{
    JsonDocument doc = Device.getTransmits();
    for (const JsonPairConst &pair : doc.as<JsonObjectConst>())
    {
        const size_t length = measureJson(pair.value());
        char *payload = new char[length + 1];
        serializeJson(pair.value(), payload, length + 1);
        client->send(payload, pair.key().c_str());
        delete[] payload;
    }
}

#endif // EXTENSION_SERVERSENTEVENTS
