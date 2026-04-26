#pragma once

#if MODE_STREAM

#include "modules/ModeModule.h"

#include <AsyncUDP.h>
#include <bits/unique_ptr.h>

class StreamMode final : public ModeModule
{
private:
    static inline uint16_t port = 4048;

    AsyncUDP udp{};

    void set(uint16_t _port);
    void transmit();

    static void onPacket(AsyncUDPPacket packet);

public:
    static constexpr std::string_view name{"Stream"};

    explicit StreamMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_STREAM
