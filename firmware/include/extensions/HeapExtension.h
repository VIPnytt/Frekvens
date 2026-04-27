#pragma once

#if EXTENSION_HEAP

#include "modules/ExtensionModule.h"

class HeapExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Heap"};

    unsigned long lastMillis = 0;

public:
    explicit HeapExtension() : ExtensionModule(name) {};

    void handle() override;
    void transmit();

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_HEAP
