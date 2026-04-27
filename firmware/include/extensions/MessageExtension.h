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
    static constexpr std::string_view name{"Message"};

    bool active = false;
    bool pending = false;

    uint8_t offsetY = 0;
    uint8_t repeat = 3;
    uint8_t width = 0;

    int16_t offsetX = GRID_COLUMNS;

    unsigned long lastMillis = 0;

#if FONT_SMALL
    std::string fontName{SmallFont::name};
#else
    std::string fontName{Fonts.names[0]};
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
    explicit MessageExtension() : ExtensionModule(name) {};

    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_MESSAGE
