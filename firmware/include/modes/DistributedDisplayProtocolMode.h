#pragma once

#include "config/constants.h"

#if MODE_DISTRIBUTEDDISPLAYPROTOCOL

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class DistributedDisplayProtocolMode : public ModeModule
{
private:
    static constexpr std::string_view _name = "Distributed Display Protocol";

    std::unique_ptr<AsyncUDP> udp;

public:
    DistributedDisplayProtocolMode() : ModeModule(_name.data()) {};

    void wake() override;
    void sleep() override;

    static void onPacket(AsyncUDPPacket packet);
};

#endif // MODE_DISTRIBUTEDDISPLAYPROTOCOL
