#if MODE_CIRCLE

#include "extensions/MicrophoneExtension.h"
#include "modes/CircleMode.h"
#include "services/DisplayService.h"

void CircleMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 6) && Microphone->isTriggered())
#else
    if (millis() - lastMillis > (1 << 6))
#endif // EXTENSION_MICROPHONE
    {
        uint8_t _radius = radius;
        bool _lit = lit;
        do
        {
            Display.drawEllipse(x, y, _radius, 1.25, false, _lit ? UINT8_MAX : 0);
            _lit = !_lit;
            _radius += 3;
        } while (_radius <= maxRadius);
        lastMillis = millis();
        ++radius;
        if (radius > 3)
        {
            radius = 1;
            lit = !lit;
        }
    }
}

#endif // MODE_CIRCLE
