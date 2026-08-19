#pragma once

#if EXTENSION_BUTTON

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ExtensionModule.h"

class ButtonExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Button"};

    enum class State : uint8_t // NOLINT(performance-enum-size)
    {
        NONE,
        PRESS,
        HOLD,
    };

#ifdef PIN_SW1
    bool brightnessIncrease{false};
#endif

#ifdef PIN_SW1
    unsigned long powerMillis{0U};
#endif
#ifdef PIN_SW2
    unsigned long modeMillis{0U};
#endif

#ifdef PIN_SW1
    State powerState{State::NONE};
#endif
#ifdef PIN_SW2
    State modeState{State::NONE};
#endif

#ifdef PIN_SW1
    static inline bool powerInput{false};
#endif
#ifdef PIN_SW2
    static inline bool modeInput{false};
#endif

#ifdef PIN_SW1
    static IRAM_ATTR void onChangePower();
#endif
#ifdef PIN_SW2
    static IRAM_ATTR void onChangeMode();
#endif

    static void event(const char *key, const char *value);

public:
    explicit ButtonExtension() : ExtensionModule(name) {};

    void configure() override;
    void handle() override;

#ifdef PIN_SW1
    void handlePower();
#endif
#ifdef PIN_SW2
    void handleMode();
#endif

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_BUTTON
