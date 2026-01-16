#pragma once

#if MODE_STREAM

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class StreamMode : public ModeModule
{
private:
    static constexpr std::string_view _name = "Stream";

    std::unique_ptr<AsyncUDP> udp;

public:
    StreamMode() : ModeModule(_name.data()) {};

    void begin() override;
    void end() override;

    static void onPacket(AsyncUDPPacket packet);
};

#endif // MODE_STREAM
