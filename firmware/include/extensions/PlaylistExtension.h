#pragma once

#if EXTENSION_PLAYLIST

#include "modules/ExtensionModule.h"

#include <span>
#include <vector>

class PlaylistExtension final : public ExtensionModule
{
public:
    explicit PlaylistExtension() : ExtensionModule(name) {};

    struct Mode
    {
        std::string mode = "";
        uint16_t duration = 60;
    };

    void configure() override;
    void begin() override;
    void handle() override;
    [[nodiscard]] bool getActive() const;
    void setActive(bool active);

    void onReceive(JsonObjectConst payload, std::string_view source) override;
    void onTransmit(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif

private:
    static constexpr std::string_view name{"Playlist"};

    bool active = false;

    uint8_t step = 0;

    unsigned long lastMillis = 0;

    std::vector<Mode> playlist{};

    void setPlaylist(std::span<Mode> modes);
    void transmit();
};

#endif // EXTENSION_PLAYLIST
