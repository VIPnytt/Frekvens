#pragma once

#if EXTENSION_STATUSLED

#include "modules/ExtensionModule.h"

class StatusLedExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Status LED"};

    bool critical{false};

    unsigned long lastMillis{0};
    unsigned long lastMillisWarning{0};

public:
    explicit StatusLedExtension() : ExtensionModule(name) {};

    void configure() override;
    void handle() override;

    void error();
    void warning();
};

#endif // EXTENSION_STATUSLED
