#pragma once

#include "config/constants.h"

#if EXTENSION_BUTTON

#include "modules/ExtensionModule.h"

class ButtonExtension : public ExtensionModule
{
private:
#ifdef PIN_SW1
    bool
        brightnessIncrease = false,
        powerLong = false,
        powerShort = false;
#endif
#ifdef PIN_SW2
    bool
        modeLong = false,
        modeShort = false;
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
    ButtonExtension();

    void configure() override;
    void handle() override;
};

extern ButtonExtension *Button;

#endif // EXTENSION_BUTTON
