#include "config/constants.h"
#include "modes/E131Mode.h"
#include "services/DisplayService.h"

void E131Mode::wake()
{
    if (!udp)
    {
        udp = std::make_unique<AsyncUDP>();
        if (udp->listen(5568))
        {
            udp->onPacket(&onPacket);
#ifdef F_DEBUG
            Serial.printf("%s: listening at port 5568\n", name);
#endif
        }
    }
}

void E131Mode::onPacket(AsyncUDPPacket packet)
{
    switch (packet.length())
    {
    case (60 + COLUMNS * ROWS):
        Display.setFrame(packet.data() + 60);
        break;
    case (126 + COLUMNS * ROWS):
        Display.setFrame(packet.data() + 126);
        break;
    case (60 + 1):
        Display.clear(packet.data()[60]);
        break;
    case (126 + 1):
        Display.clear(packet.data()[126]);
        break;
#ifdef F_DEBUG
    default:
        Serial.printf("%s: unsupported package received\n", "E1.31");
#endif
    }
}

void E131Mode::sleep()
{
    udp.reset();
}
