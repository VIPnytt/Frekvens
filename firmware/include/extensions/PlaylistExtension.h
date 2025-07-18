#pragma once

#include "config/constants.h"

#if EXTENSION_PLAYLIST

#include <ArduinoJson.h>
#include <vector>

#include "modules/ExtensionModule.h"

class PlaylistExtension : public ExtensionModule
{
public:
    PlaylistExtension();

    struct Item
    {
        String mode;
        uint16_t duration;
    };

    void setup() override;
    void ready() override;
    void handle() override;
    bool get();
    void set(bool enable);

    void receiverHook(const JsonDocument doc) override;
    void transmitterHook(const JsonDocument &doc, const char *const source) override;

private:
    bool active = false;
    std::vector<Item> playlist = {};
    unsigned long lastMillis = 0;
    uint8_t step = 0;

    void load(std::vector<Item> modes);
    void transmit();
};

extern PlaylistExtension *Playlist;

#endif // EXTENSION_PLAYLIST
