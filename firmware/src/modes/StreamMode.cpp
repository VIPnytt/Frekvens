#if MODE_STREAM

#include "modes/StreamMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

#include <Preferences.h>
#include <span>

void StreamMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    const HomeAssistantExtension &_ha = Extensions.HomeAssistant();
    {
        const std::string id{std::string(name).append("_protocol")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(
            R"({"port":{{{"Art-Net":6454,"Distributed Display Protocol":4048,"E1.31":5568}.get(value)}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:protocol");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" protocol"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        options.add("Art-Net");
        options.add("Distributed Display Protocol");
        options.add("E1.31");
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set(
            R"({{{4048:"Distributed Display Protocol",5568:"E1.31",6454:"Art-Net"}.get(value_json.port)}})");
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name.data(), true);
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
        ESP_LOGD(name, "listening at " HOSTNAME ".local:%d", port); // NOLINT(cppcoreguidelines-avoid-do-while)
    }
}

void StreamMode::set(uint16_t _port)
{
    if (_port != port && (_port == 4048 || _port == 5568 || _port == 6454))
    {
        port = _port;
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putUShort("port", port);
        Storage.end();
        if (udp)
        {
            udp->listen(port);
            ESP_LOGD(name, "listening at " HOSTNAME ".local:%d", port); // NOLINT(cppcoreguidelines-avoid-do-while)
        }
        transmit();
    }
}

void StreamMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["port"].set(port);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void StreamMode::onReceive(JsonObjectConst payload,
                           std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Port
    if (payload["port"].is<uint16_t>())
    {
        set(payload["port"].as<uint16_t>());
    }
}

void StreamMode::onPacket(AsyncUDPPacket packet)
{
    const uint16_t port = packet.localPort();
    const size_t len = packet.length();
    if ((port == 4048 && (len == 10 + (GRID_COLUMNS * GRID_ROWS) || len == 14 + (GRID_COLUMNS * GRID_ROWS))) ||
        (port == 6454 && len == 18 + (GRID_COLUMNS * GRID_ROWS)) ||
        (port == 5568 && len == 126 + (GRID_COLUMNS * GRID_ROWS)))
    {
        Display.setFrame(std::span<const uint8_t>(packet.data(), len).last(GRID_COLUMNS * GRID_ROWS));
    }
}

void StreamMode::end() { udp.reset(); }

#endif // MODE_STREAM
