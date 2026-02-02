#pragma once

#if MODE_STREAM

#include "modules/ModeModule.h"

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

class StreamMode final : public ModeModule
{
private:
    static constexpr std::string_view _name = "Stream";

    uint16_t port = 4048;

    std::unique_ptr<AsyncUDP> udp = nullptr;

    void set(uint16_t _port);
    void transmit();

    static void onPacket(AsyncUDPPacket packet);

protected:
    ~StreamMode() = default;

public:
    StreamMode() : ModeModule(_name.data()) {};
    StreamMode(const StreamMode &) = delete;
    StreamMode &operator=(const StreamMode &) = delete;

    void configure() override;
    void begin() override;
    void end() override;

    void onReceive(const JsonDocument &doc, const char *source) override;
};

#endif // MODE_STREAM
