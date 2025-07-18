#pragma once

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class ArtNetMode : public ModeModule
{
private:
    std::unique_ptr<AsyncUDP> udp;

public:
    ArtNetMode() : ModeModule("Art-Net") {};

    void wake() override;
    void sleep() override;

    static void onPacket(AsyncUDPPacket packet);
};
