#pragma once

#if EXTENSION_MESSAGE

#include "config/constants.h"
#include "handlers/TextHandler.h"
#include "modules/ExtensionModule.h"
#include "services/FontsService.h"

#include <bits/unique_ptr.h>

class MessageExtension final : public ExtensionModule
{
private:
    bool active = false;
    bool pending = false;

    int8_t offsetY = 0;

    uint8_t frame[GRID_COLUMNS * GRID_ROWS] = {0};
    uint8_t repeat = 3;

    int16_t offsetX = GRID_COLUMNS;
    int16_t width = 0;

    unsigned long lastMillis = 0;

    FontModule *font = nullptr;

    std::vector<std::string> messages = {};

    std::unique_ptr<TextHandler> text = {};

    void addMessage(std::string message);

    void setFont(const char *fontName);
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

extern MessageExtension *Message;

#endif // EXTENSION_MESSAGE
