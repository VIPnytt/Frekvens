#pragma once

#if EXTENSION_PLAYLIST

#include "modules/ExtensionModule.h"

#include <span>
#include <vector>

class PlaylistExtension final : public ExtensionModule
{
public:
    explicit PlaylistExtension();

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

    void onReceive(JsonObjectConst payload, const char *source) override;
    void onTransmit(JsonObjectConst payload, const char *source) override;

private:
    bool active = false;

    uint8_t step = 0;

    unsigned long lastMillis = 0;

    std::vector<Mode> playlist{};

    void setPlaylist(std::span<Mode> modes);
    void transmit();
};

extern PlaylistExtension *Playlist; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

#endif // EXTENSION_PLAYLIST
