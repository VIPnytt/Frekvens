#pragma once

#include "config/constants.h"

#if MODE_E131

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class E131Mode : public ModeModule
{
private:
    static constexpr std::string_view _name = "E1.31";

    std::unique_ptr<AsyncUDP> udp;

public:
    E131Mode() : ModeModule(_name.data()) {};

    void begin() override;
    void end() override;

    static void onPacket(AsyncUDPPacket packet);
};

#endif // MODE_E131
