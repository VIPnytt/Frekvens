#pragma once

#if MODE_TICKER

#include "config/constants.h"     // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <bits/unique_ptr.h>

class TickerMode final : public ModeModule
{
private:
    bool pending = false;

    int8_t offsetY = GRID_ROWS / 2;

    int16_t offsetX = GRID_COLUMNS;
    int16_t width = 0;

    unsigned long lastMillis = 0;

    std::string message = NAME;

    std::unique_ptr<const FontModule> font{};

    std::unique_ptr<TextHandler> text{};

    void setFont(std::string_view fontName);
    void setMessage(std::string_view _message);

    void transmit();

public:
    static constexpr std::string_view name{"Ticker"};

    explicit TickerMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;
    void end() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_TICKER
