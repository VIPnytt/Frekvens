#pragma once

#if EXTENSION_BUTTON

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ExtensionModule.h"

class ButtonExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Button"};

#ifdef PIN_SW1
    bool brightnessIncrease = false;
    static inline bool powerLong = false;
    static inline bool powerShort = false;
#endif
#ifdef PIN_SW2
    static inline bool modeLong = false;
    static inline bool modeShort = false;
#endif

#ifdef PIN_SW1
    static inline volatile bool powerState = false;
#endif
#ifdef PIN_SW2
    static inline volatile bool modeState = false;
#endif

#ifdef PIN_SW1
    static inline volatile unsigned long powerMillis = 0;
#endif
#ifdef PIN_SW2
    static inline volatile unsigned long modeMillis = 0;
#endif

    static IRAM_ATTR void onInterrupt();

    static void event(const char *key, const char *value);

public:
    explicit ButtonExtension() : ExtensionModule(name) {};

    void configure() override;
    void handle() override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_BUTTON
