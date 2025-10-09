#pragma once

#include "config/constants.h"

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

    void wake() override;
    void sleep() override;

    static void onPacket(AsyncUDPPacket packet);
};

#endif // MODE_ARTNET