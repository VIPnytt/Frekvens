#pragma once

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class E131Mode : public ModeModule
{
private:
    std::unique_ptr<AsyncUDP> udp;

public:
    E131Mode() : ModeModule("E1.31") {};

    void wake() override;
    void sleep() override;

    static void onPacket(AsyncUDPPacket packet);
};
