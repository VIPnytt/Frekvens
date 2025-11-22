#pragma once

#include "config/constants.h"

#if EXTENSION_PLAYLIST

#include <vector>

#include "modules/ExtensionModule.h"

class PlaylistExtension : public ExtensionModule
{
public:
    PlaylistExtension();

    struct Mode
    {
        std::string mode;
        uint16_t duration;
    };

    void setup() override;
    void ready() override;
    void handle() override;
    bool getActive() const;
    void setActive(bool active, const char *const source);

    void receiverHook(const JsonDocument doc, const char *const source) override;
    void transmitterHook(const JsonDocument &doc, const char *const source) override;

private:
    bool active = false;

    uint8_t step = 0;

    unsigned long lastMillis = 0;

    std::vector<Mode> playlist = {};

    void setPlaylist(std::vector<Mode> modes);
    void transmit();
};

extern PlaylistExtension *Playlist;

#endif // EXTENSION_PLAYLIST
