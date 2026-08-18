#if MODE_STREAM

#include "modes/StreamMode.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <nvs.h>

/**
 * @brief Loads the persisted streaming port and publishes the current configuration.
 */
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

/**
 * @brief Starts UDP listening on the configured port and registers its protocol handler.
 */
void StreamMode::begin()
{
    if (udp.listen(port))
    {
        if (port == 4048U)
        {
            udp.onPacket(&onDistributedDisplayProtocol);
        }
        else if (port == 5568U)
        {
            udp.onPacket(&onE131);
        }
        else if (port == 6454U)
        {
            udp.onPacket(&onArtNet);
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGD(name.data(), "listening at " HOSTNAME ".local:%u", port);
    }
}

/**
 * @brief Sets the streaming protocol port and restarts UDP listening.
 *
 * Persists the port when it is supported, then publishes the updated configuration.
 *
 * @param _port Supported streaming port: 4048, 5568, or 6454.
 */
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

/**
 * @brief Publishes the configured streaming port.
 */
void StreamMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["port"].set(port);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Applies a received streaming port configuration.
 *
 * @param payload Configuration payload containing the optional `port` value.
 * @param source Source identifier for the received configuration.
 */
void StreamMode::onReceive(JsonObjectConst payload,
                           std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Port
    if (payload["port"].is<uint16_t>())
    {
        set(payload["port"].as<uint16_t>());
    }
}

/**
 * @brief Processes an Art-Net packet containing one complete display frame.
 *
 * @param packet UDP packet with an 18-byte Art-Net header followed by the frame data.
 */
void StreamMode::onArtNet(AsyncUDPPacket packet)
{
    if (packet.length() == 18U + (GRID_COLUMNS * GRID_ROWS))
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(18U)));
    }
}

/**
 * @brief Processes a Distributed Display Protocol packet and updates the display frame.
 *
 * @param packet UDP packet containing a supported protocol header followed by a complete display frame.
 */
void StreamMode::onDistributedDisplayProtocol(AsyncUDPPacket packet)
{
    const std::span<const uint8_t> data{std::span(packet.data(), packet.length())};
    if (packet.length() == 10U + (GRID_COLUMNS * GRID_ROWS) && (data.front() & (0b1U << 4U)) == 0U)
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(10U)));
    }
    else if (packet.length() == 14U + (GRID_COLUMNS * GRID_ROWS) && (data.front() & (0b1U << 4U)) != 0U)
    {
        Display.setFrame(static_cast<std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS>>(
            std::span(packet.data(), packet.length()).subspan(14U)));
    }
}

/**
 * @brief Processes an E1.31 packet and updates the display frame.
 *
 * @param packet UDP packet containing a 126-byte E1.31 header followed by a complete display frame.
 */
void StreamMode::onE131(AsyncUDPPacket packet)
{
    if (packet.length() == 126U + (GRID_COLUMNS * GRID_ROWS))
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
