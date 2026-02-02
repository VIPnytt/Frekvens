#pragma once

#if EXTENSION_HEAP

#include "modules/ExtensionModule.h"

class HeapExtension final : public ExtensionModule
{
private:
    unsigned long lastMillis = 0;

protected:
    ~HeapExtension() = default;

public:
    HeapExtension();
    HeapExtension(const HeapExtension &) = delete;
    HeapExtension &operator=(const HeapExtension &) = delete;

#if EXTENSION_HOMEASSISTANT
    void configure() override;
#endif // EXTENSION_HOMEASSISTANT

    void handle() override;
    void transmit();
};

extern HeapExtension *Heap;

#endif // EXTENSION_HEAP
