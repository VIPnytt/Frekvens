#pragma once

#if MODE_STREAM

#include "modules/ModeModule.h"

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

class StreamMode final : public ModeModule
{
private:
    uint16_t port = 4048;

    std::unique_ptr<AsyncUDP> udp{};

    void set(uint16_t _port);
    void transmit();

    static void onPacket(AsyncUDPPacket packet);

public:
    static constexpr std::string_view name{"Stream"};

    explicit StreamMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void end() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_STREAM
