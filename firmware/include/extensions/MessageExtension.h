#pragma once

#if EXTENSION_MESSAGE

#include "config/constants.h"     // NOLINT(misc-include-cleaner)
#include "fonts/SmallFont.h"      // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "modules/ExtensionModule.h"

#include <array>
#include <bits/unique_ptr.h>

class MessageExtension final : public ExtensionModule
{
private:
    bool active = false;
    bool pending = false;

    uint8_t offsetY = 0;
    uint8_t repeat = 3;
    uint8_t width = 0;

    int16_t offsetX = GRID_COLUMNS;

    unsigned long lastMillis = 0;

#if FONT_SMALL
    std::string fontName = SmallFont::name.data();
#else
    std::string fontName = Fonts.names[0].data();
#endif // FONT_SMALL

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};

    std::unique_ptr<const FontModule> font{};

    std::unique_ptr<TextHandler> text{};

    std::vector<std::string> messages{};

    void addMessage(std::string message);

    void setFont(std::string_view _fontName);
    void setRepeat(uint8_t count);

    void transmit();

public:
    explicit MessageExtension();

#if EXTENSION_HOMEASSISTANT
    void configure() override;
#endif

    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

extern MessageExtension *Message; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

#endif // EXTENSION_MESSAGE
