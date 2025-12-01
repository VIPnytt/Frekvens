#pragma once

#if EXTENSION_MESSAGE

#include <bits/unique_ptr.h>

#include "config/constants.h"
#include "handlers/TextHandler.h"
#include "modules/ExtensionModule.h"
#include "services/FontsService.h"

class MessageExtension : public ExtensionModule
{
private:
    bool
        active = false,
        pending = false;

    int8_t offsetY = 0;

    int16_t
        offsetX = GRID_COLUMNS,
        width = 0;

    uint8_t
        frame[GRID_COLUMNS * GRID_ROWS],
        repeat = 3;

    unsigned long lastMillis = 0;

    FontModule *font = nullptr;

    std::vector<std::string> messages;

    std::unique_ptr<TextHandler> text;

    void addMessage(std::string message);

    void setFont(const char *const fontName);
    void setRepeat(uint8_t count);

    void transmit();

public:
    MessageExtension();

#if EXTENSION_HOMEASSISTANT
    void configure() override;
#endif

    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

extern MessageExtension *Message;

#endif // EXTENSION_MESSAGE
