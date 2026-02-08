#pragma once

#if EXTENSION_BUTTON

#include "config/constants.h"
#include "modules/ExtensionModule.h"

class ButtonExtension final : public ExtensionModule
{
private:
#ifdef PIN_SW1
    bool brightnessIncrease = false;
    bool powerLong = false;
    bool powerShort = false;
#endif
#ifdef PIN_SW2
    bool modeLong = false;
    bool modeShort = false;
#endif

#ifdef PIN_SW1
    volatile bool powerState = false;
#endif
#ifdef PIN_SW2
    volatile bool modeState = false;
#endif

#ifdef PIN_SW1
    volatile unsigned long powerMillis = 0;
#endif
#ifdef PIN_SW2
    volatile unsigned long modeMillis = 0;
#endif

    static IRAM_ATTR void onInterrupt();

    static void event(const char *key, const char *value);

public:
    explicit ButtonExtension();

    void configure() override;
    void handle() override;
};

extern ButtonExtension *Button;

#endif // EXTENSION_BUTTON
