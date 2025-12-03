#pragma once

#if MODE_ARTNET

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class ArtNetMode : public ModeModule
{
private:
    static constexpr std::string_view _name = "Art-Net";

    std::unique_ptr<AsyncUDP> udp;

public:
    ArtNetMode() : ModeModule(_name.data()) {};

    void begin() override;
    void end() override;

    static void onPacket(AsyncUDPPacket packet);
};

#endif // MODE_ARTNET
