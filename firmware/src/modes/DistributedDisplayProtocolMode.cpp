#include "config/constants.h"
#include "modes/DistributedDisplayProtocolMode.h"
#include "services/DisplayService.h"

void DistributedDisplayProtocolMode::wake()
{
    if (!udp)
    {
        udp = std::make_unique<AsyncUDP>();
        if (udp->listen(4048))
        {
            udp->onPacket(&onPacket);
#ifdef F_DEBUG
            Serial.print(name);
            Serial.println(": listening at port 4048");
#endif
        }
    }
}

void DistributedDisplayProtocolMode::onPacket(AsyncUDPPacket packet)
{
    switch (packet.length())
    {
    case (10 + COLUMNS * ROWS):
        Display.setFrame(packet.data() + 10);
        break;
    case (14 + COLUMNS * ROWS):
        Display.setFrame(packet.data() + 14);
        break;
    case (10 + 1):
        Display.clear(packet.data()[10]);
        break;
    case (14 + 1):
        Display.clear(packet.data()[14]);
        break;
#ifdef F_DEBUG
    default:
        Serial.println("Distributed Display Protocol: unsupported package received");
#endif
    }
}

void DistributedDisplayProtocolMode::sleep()
{
    udp.reset();
}
