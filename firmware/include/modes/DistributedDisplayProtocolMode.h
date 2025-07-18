#pragma once

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class DistributedDisplayProtocolMode : public ModeModule
{
private:
    std::unique_ptr<AsyncUDP> udp;

public:
    DistributedDisplayProtocolMode() : ModeModule("Distributed Display Protocol") {};

    void wake() override;
    void sleep() override;
    
    static void onPacket(AsyncUDPPacket packet);
};
