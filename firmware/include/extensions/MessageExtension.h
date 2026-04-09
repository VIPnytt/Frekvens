#pragma once

#if EXTENSION_MESSAGE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"
#include "modules/ExtensionModule.h"

#include <array>
#include <bits/unique_ptr.h>

class MessageExtension final : public ExtensionModule
{
private:
    bool active = false;
    bool pending = false;

    int8_t offsetY = 0;

    uint8_t repeat = 3;

    int16_t offsetX = GRID_COLUMNS;
    int16_t width = 0;

    unsigned long lastMillis = 0;

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};

    std::unique_ptr<FontModule> font{};

    std::unique_ptr<TextHandler> text{};

    std::vector<std::string> messages{};

    void addMessage(std::string message);

    void setFont(std::string_view fontName);
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
