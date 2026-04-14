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

#if EXTENSION_HOMEASSISTANT
    void configure() override;
#endif // EXTENSION_HOMEASSISTANT

    void handle() override;
    void transmit();
};

#endif // EXTENSION_HEAP
