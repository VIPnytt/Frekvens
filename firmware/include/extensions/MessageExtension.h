#pragma once

#include "config/constants.h"

#if EXTENSION_MESSAGE

#include <ArduinoJson.h>
#include <bits/unique_ptr.h>

#include "handlers/TextHandler.h"
#include "modules/ExtensionModule.h"
#include "services/FontsService.h"

class MessageExtension : public ExtensionModule
{
private:
    bool
        active = false,
        pending = false;

    int8_t offsetY;

    int16_t
        offsetX,
        textWidth;

    uint8_t
        frame[COLUMNS * ROWS],
        repeat = 3;

    unsigned long lastMillis = 0;

    FontModule *font = nullptr;

    std::vector<String> messages;

    std::unique_ptr<TextHandler> text;

    void setFont(const char *const fontName);
    void setRepeat(uint8_t count);

    void transmit();

public:
    MessageExtension();

#if EXTENSION_HOMEASSISTANT
    void setup() override;
#endif

    void ready() override;
    void handle() override;
    void receiverHook(const JsonDocument doc) override;
};

extern MessageExtension *Message;

#endif // EXTENSION_MESSAGE
