#if MODE_STREAM

#include <ESPmDNS.h>
#include <Preferences.h>

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "modes/StreamMode.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

void StreamMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_protocol");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"protocol\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:protocol";
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" protocol");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[HomeAssistantAbbreviations::options].to<JsonArray>();
        options.add(artnet);
        options.add(ddp);
        options.add(e131);
        component[HomeAssistantAbbreviations::platform] = "select";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.protocol}}";
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("port"))
    {
        port = Storage.getUShort("port");
    }
    Storage.end();
    transmit();
}

void StreamMode::begin()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(port))
    {
        udp->onPacket(&onPacket);
        ESP_LOGD(name, "listening at " HOSTNAME ".local:%d", port);
    }
}

void StreamMode::setPort(const uint16_t _port)
{
    if (_port != port && (_port == 4048 || _port == 5568 || _port == 6454))
    {
        port = _port;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("port", port);
        Storage.end();
        transmit();
        if (udp)
        {
            udp->listen(port);
            ESP_LOGD(name, "listening at " HOSTNAME ".local:%d", port);
        }
    }
}

void StreamMode::setProtocol(const char *const protocol)
{
    if (!strcmp(protocol, artnet.data()))
    {
        setPort(6454);
    }
    else if (!strcmp(protocol, ddp.data()))
    {
        setPort(4048);
    }
    else if (!strcmp(protocol, e131.data()))
    {
        setPort(5568);
    }
}

void StreamMode::transmit()
{
    JsonDocument doc;
    doc["port"] = port;
    switch (port)
    {
    case 4048:
        doc["protocol"] = ddp;
        break;
    case 5568:
        doc["protocol"] = e131;
        break;
    case 6454:
        doc["protocol"] = artnet;
        break;
    }
    Device.transmit(doc, name);
}

void StreamMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Port
    if (doc["port"].is<uint16_t>())
    {
        setPort(doc["port"].as<uint16_t>());
    }
    // Protocol
    if (doc["protocol"].is<const char *>())
    {
        setProtocol(doc["protocol"].as<const char *>());
    }
}

void StreamMode::onPacket(AsyncUDPPacket packet)
{
    const uint16_t port = packet.localPort();
    const size_t len = packet.length();
    if ((port == 4048 && (len == 10 + GRID_COLUMNS * GRID_ROWS || len == 14 + GRID_COLUMNS * GRID_ROWS)) ||
        (port == 6454 && len == 18 + GRID_COLUMNS * GRID_ROWS) ||
        (port == 5568 && len == 126 + GRID_COLUMNS * GRID_ROWS))
    {
        Display.setFrame(packet.data() + len - GRID_COLUMNS * GRID_ROWS);
    }
}

void StreamMode::end()
{
    udp.reset();
}

#endif // MODE_STREAM
