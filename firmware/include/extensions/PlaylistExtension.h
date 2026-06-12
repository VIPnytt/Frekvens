#pragma once

#if EXTENSION_PLAYLIST

#include "modules/ExtensionModule.h"

#include <span>
#include <vector>

class PlaylistExtension final : public ExtensionModule
{
public:
    explicit PlaylistExtension() : ExtensionModule(name) {};

    void begin() override;
    void handle() override;
    [[nodiscard]] bool getActive() const;
    void setActive(bool _active);

    void onReceive(JsonObjectConst payload, std::string_view source) override;
    void onTransmit(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif

private:
    static constexpr std::string_view name{"Playlist"};

    bool active{false};

    uint8_t step{0U};

    uint32_t duration{0U};

    unsigned long lastMillis{0UL};

    std::string mode{""};

    void setPlaylist(std::span<const std::pair<std::string, uint16_t>> playlist);
    void transmit();
};

#endif // EXTENSION_PLAYLIST
