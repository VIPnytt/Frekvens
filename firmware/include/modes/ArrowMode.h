#pragma once

#if MODE_ARROW

#include "modules/ModeModule.h"

#include <array>
#include <span>

class ArrowMode final : public ModeModule
{
private:
    static constexpr std::array<uint16_t, 15> arrow0{
        0b000000010000000,
        0b000000010000000,
        0b000000000000000,
        0b000000000000000,
        0b000000000000000,
        0b000000010000000,
        0b000000010000000,
        0b000000000000000,
        0b000000000000000,
        0b100000000000001,
        0b000000010000000,
        0b000100010001000,
        0b000000000000000,
        0b000001000100000,
        0b000000000000000,
    };
    static constexpr std::array<uint16_t, 14> arrow1{
        0b0000001000000,
        0b0000001000000,
        0b0000000000000,
        0b0000000000000,
        0b0000000000000,
        0b0000000000000,
        0b0000001000000,
        0b0000001000000,
        0b0000000000000,
        0b1000000000001,
        0b0000000000000,
        0b0010000000100,
        0b0000001000000,
        0b0000101010000,
    };
    static constexpr std::array<uint16_t, 11> arrow2{
        0b00000100000,
        0b00000100000,
        0b00000000000,
        0b00000000000,
        0b00000000000,
        0b00000000000,
        0b00000100000,
        0b10000100001,
        0b00000000000,
        0b00100000100,
        0b00000000000,
    };
    static constexpr std::array<uint16_t, 8> arrow3{
        0b000010000,
        0b000010000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b100000001,
        0b000010000,
        0b001010100,
    };
    static constexpr std::array<uint8_t, 5> arrow4{
        0b0001000,
        0b0001000,
        0b0000000,
        0b1000001,
        0b0000000,
    };
    static constexpr std::array<uint8_t, 2> arrow5{
        0b00100,
        0b10101,
    };

    static constexpr std::array<std::span<const uint16_t>, 4> arrows0{
        arrow0,
        arrow1,
        arrow2,
        arrow3,
    };

    static constexpr std::array<std::span<const uint8_t>, 2> arrows1{
        arrow4,
        arrow5,
    };

    bool direction = true;

    uint8_t index = 0;

    unsigned long lastMillis = 0;

public:
    explicit ArrowMode() : ModeModule("Arrow") {};

    void handle() override;
};

#endif // MODE_ARROW
