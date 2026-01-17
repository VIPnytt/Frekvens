#pragma once

#if MODE_STREAM

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

#include "modules/ModeModule.h"

class StreamMode : public ModeModule
{
private:
    static constexpr std::string_view _name = "Stream";

    uint16_t port = 4048;

    std::unique_ptr<AsyncUDP> udp;

    void setPort(const uint16_t _port);
    void setProtocol(const char *const protocol);
    void transmit();

public:
    StreamMode() : ModeModule(_name.data()) {};

    void configure() override;
    void begin() override;
    void end() override;

    void onReceive(const JsonDocument doc, const char *const source) override;

    static void onArtNet(AsyncUDPPacket packet);
    static void onDistributedDisplayProtocol(AsyncUDPPacket packet);
    static void onE131(AsyncUDPPacket packet);
};

#endif // MODE_STREAM
