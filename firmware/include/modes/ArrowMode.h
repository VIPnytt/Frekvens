#pragma once

#if MODE_ARROW

#include "modules/ModeModule.h"

#include <array>
#include <span>

class ArrowMode final : public ModeModule
{
private:
    static constexpr std::array<uint16_t, 15U> arrow0{
        0b000000010000000U,
        0b000000010000000U,
        0b000000000000000U,
        0b000000000000000U,
        0b000000000000000U,
        0b000000010000000U,
        0b000000010000000U,
        0b000000000000000U,
        0b000000000000000U,
        0b100000000000001U,
        0b000000010000000U,
        0b000100010001000U,
        0b000000000000000U,
        0b000001000100000U,
        0b000000000000000U,
    };
    static constexpr std::array<uint16_t, 14U> arrow1{
        0b0000001000000U,
        0b0000001000000U,
        0b0000000000000U,
        0b0000000000000U,
        0b0000000000000U,
        0b0000000000000U,
        0b0000001000000U,
        0b0000001000000U,
        0b0000000000000U,
        0b1000000000001U,
        0b0000000000000U,
        0b0010000000100U,
        0b0000001000000U,
        0b0000101010000U,
    };
    static constexpr std::array<uint16_t, 11U> arrow2{
        0b00000100000U,
        0b00000100000U,
        0b00000000000U,
        0b00000000000U,
        0b00000000000U,
        0b00000000000U,
        0b00000100000U,
        0b10000100001U,
        0b00000000000U,
        0b00100000100U,
        0b00000000000U,
    };
    static constexpr std::array<uint16_t, 8U> arrow3{
        0b000010000U,
        0b000010000U,
        0b000000000U,
        0b000000000U,
        0b000000000U,
        0b100000001U,
        0b000010000U,
        0b001010100U,
    };
    static constexpr std::array<uint8_t, 5U> arrow4{
        0b0001000U,
        0b0001000U,
        0b0000000U,
        0b1000001U,
        0b0000000U,
    };
    static constexpr std::array<uint8_t, 2U> arrow5{
        0b00100U,
        0b10101U,
    };

    static constexpr std::array<std::span<const uint16_t>, 4U> arrows0{
        arrow0,
        arrow1,
        arrow2,
        arrow3,
    };

    static constexpr std::array<std::span<const uint8_t>, 2U> arrows1{
        arrow4,
        arrow5,
    };

    bool direction{true};

    uint8_t index{0U};

    unsigned long lastMillis{0UL};

public:
    static constexpr std::string_view name{"Arrow"};

    explicit ArrowMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_ARROW
