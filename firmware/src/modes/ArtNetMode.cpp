#include "config/constants.h"
#include "modes/ArtNetMode.h"
#include "services/DisplayService.h"

void ArtNetMode::wake()
{
    if (!udp)
    {
        udp = std::make_unique<AsyncUDP>();
        if (udp->listen(6454))
        {
            udp->onPacket(&onPacket);
#ifdef F_DEBUG
            Serial.print(name);
            Serial.println(": listening at port 6454");
#endif
        }
    }
}

void ArtNetMode::onPacket(AsyncUDPPacket packet)
{
    switch (packet.length())
    {
    case (18 + COLUMNS * ROWS):
        Display.setFrame(packet.data() + 18);
        break;
    case (18 + 1):
        Display.clear(packet.data()[18]);
        break;
#ifdef F_DEBUG
    default:
        Serial.println("Art-Net: unsupported package received");
#endif
    }
}

void ArtNetMode::sleep()
{
    udp.reset();
}
