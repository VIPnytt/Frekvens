#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "modes/CircleMode.h"
#include "services/DisplayService.h"

void CircleMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > 30 && Microphone->play())
#else
    if (millis() - lastMillis > 30)
#endif
    {
        uint8_t _radius = radius;
        bool _lit = lit;
        do
        {
            Display.drawEllipse(x, y, _radius, 1, false, _lit ? UINT8_MAX : 0);
            _radius += 3;
            _lit = !_lit;
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
