#pragma once

#if FONT_BRAILLE

#include "modules/FontModule.h"

#include <array>

class BrailleFont final : public FontModule
{
private:
    static constexpr std::array<uint8_t, 26U> latinLetterA_latinLetterZ{
        // U+0031 1 DIGIT ONE
        // U+0041 A LATIN CAPITAL LETTER A
        // U+0061 a LATIN SMALL LETTER A
        0b10'00'00U,
        // U+0032 2 DIGIT TWO
        // U+0042 B LATIN CAPITAL LETTER B
        // U+0062 b LATIN SMALL LETTER B
        0b10'10'00U,
        // U+0033 3 DIGIT THREE
        // U+0043 C LATIN CAPITAL LETTER C
        // U+0063 c LATIN SMALL LETTER C
        0b11'00'00U,
        // U+0034 4 DIGIT FOUR
        // U+0044 D LATIN CAPITAL LETTER D
        // U+0064 d LATIN SMALL LETTER D
        0b11'01'00U,
        // U+0035 5 DIGIT FIVE
        // U+0045 E LATIN CAPITAL LETTER E
        // U+0065 e LATIN SMALL LETTER E
        0b10'01'00U,
        // U+0036 6 DIGIT SIX
        // U+0046 F LATIN CAPITAL LETTER F
        // U+0066 f LATIN SMALL LETTER F
        0b11'10'00U,
        // U+0037 7 DIGIT SEVEN
        // U+0047 G LATIN CAPITAL LETTER G
        // U+0067 g LATIN SMALL LETTER G
        0b11'11'00U,
        // U+0038 8 DIGIT EIGHT
        // U+0048 H LATIN CAPITAL LETTER H
        // U+0068 h LATIN SMALL LETTER H
        0b10'1100U,
        // U+0039 9 DIGIT NINE
        // U+0049 I LATIN CAPITAL LETTER I
        // U+0069 i LATIN SMALL LETTER I
        0b01'10'00U,
        // U+0030 0 DIGIT ZERO
        // U+004A J LATIN CAPITAL LETTER J
        // U+006A j LATIN SMALL LETTER J
        0b11'10'0U,
        // U+004B K LATIN CAPITAL LETTER K
        // U+006B k LATIN SMALL LETTER K
        0b10'00'10U,
        // U+004C L LATIN CAPITAL LETTER L
        // U+006C l LATIN SMALL LETTER L
        0b10'10'10U,
        // U+004D M LATIN CAPITAL LETTER M
        // U+006D m LATIN SMALL LETTER M
        0b11'00'10U,
        // U+004E N LATIN CAPITAL LETTER N
        // U+006E n LATIN SMALL LETTER N
        0b11'01'10U,
        // U+004F O LATIN CAPITAL LETTER O
        // U+006F o LATIN SMALL LETTER O
        0b10'01'10U,
        // U+0050 P LATIN CAPITAL LETTER P
        // U+0070 p LATIN SMALL LETTER P
        0b11'10'10U,
        // U+0051 Q LATIN CAPITAL LETTER Q
        // U+0071 q LATIN SMALL LETTER Q
        0b11'11'10U,
        // U+0052 R LATIN CAPITAL LETTER R
        // U+0072 r LATIN SMALL LETTER R
        0b10'11'10U,
        // U+0053 S LATIN CAPITAL LETTER S
        // U+0073 s LATIN SMALL LETTER S
        0b01'11'10U,
        // U+0054 T LATIN CAPITAL LETTER T
        // U+0074 t LATIN SMALL LETTER T
        0b11'110U,
        // U+0055 U LATIN CAPITAL LETTER U
        // U+0075 u LATIN SMALL LETTER U
        0b10'00'11U,
        // U+0056 V LATIN CAPITAL LETTER V
        // U+0076 v LATIN SMALL LETTER V
        0b10'10'11U,
        // U+0057 W LATIN CAPITAL LETTER W
        // U+0077 w LATIN SMALL LETTER W
        0b01'11'01U,
        // U+0058 X LATIN CAPITAL LETTER X
        // U+0078 x LATIN SMALL LETTER X
        0b11'00'11U,
        // U+0059 Y LATIN CAPITAL LETTER Y
        // U+0079 y LATIN SMALL LETTER Y
        0b11'01'11U,
        // U+005A Z LATIN CAPITAL LETTER Z
        // U+007A z LATIN SMALL LETTER Z
        0b10'01'11U,
    };

    [[nodiscard]] FontModule::Symbol toSymbol(uint8_t bits) const;

public:
    static constexpr std::string_view name{"Braille"};

    explicit BrailleFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_BRAILLE
