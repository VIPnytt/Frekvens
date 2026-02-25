#pragma once

#if EXTENSION_HEAP

#include "modules/ExtensionModule.h"

class HeapExtension final : public ExtensionModule
{
private:
    unsigned long lastMillis = 0;

public:
    explicit HeapExtension();

#if EXTENSION_HOMEASSISTANT
    void configure() override;
#endif // EXTENSION_HOMEASSISTANT

    void handle() override;
    void transmit();
};

extern HeapExtension *Heap; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

#endif // EXTENSION_HEAP
