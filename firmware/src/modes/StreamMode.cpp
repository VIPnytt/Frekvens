#if MODE_STREAM

#include "modes/StreamMode.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

#include <nvs.h>

void StreamMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        nvs_get_u16(handle, "port", &port);
        nvs_close(handle);
    }
    transmit();
}

void StreamMode::begin()
{
    if (udp.listen(port))
    {
        switch (port)
        {
        case 4048U:
            udp.onPacket(&onDistributedDisplayProtocol);
            break;
        case 5568U:
            udp.onPacket(&onE131);
            break;
        case 6454U:
            udp.onPacket(&onArtNet);
            break;
        default:
            return;
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGD(name.data(), "listening at " HOSTNAME ".local:%u", port);
    }
}

void StreamMode::set(uint16_t _port)
{
    if (_port != 4048U && _port != 5568U && _port != 6454U)
    {
        return;
    }
    port = _port;
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u16(handle, "port", port);
        nvs_commit(handle);
        nvs_close(handle);
    }
    begin();
    transmit();
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

void StreamMode::onArtNet(AsyncUDPPacket packet)
{
    if (packet.length() == 18U + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(18U)));
    }
}

void StreamMode::onDistributedDisplayProtocol(AsyncUDPPacket packet)
{
    const std::span<const uint8_t> data{std::span(packet.data(), packet.length())};
    const bool time{(data.front() & (0b1U << 4U)) != 0U};
    if (!time && packet.length() == 10U + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(10U)));
    }
    else if (time && packet.length() == 14U + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(14U)));
    }
}

void StreamMode::onE131(AsyncUDPPacket packet)
{
    if (packet.length() == 126U + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(126U)));
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void StreamMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_protocol")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(
            R"({"port":{{{"Art-Net":6454,"Distributed Display Protocol":4048,"E1.31":5568}.get(value)}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:protocol");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" protocol"));
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        options.add("Art-Net");
        options.add("Distributed Display Protocol");
        options.add("E1.31");
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set(
            R"({{{4048:"Distributed Display Protocol",5568:"E1.31",6454:"Art-Net"}.get(value_json.port)}})");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_STREAM
