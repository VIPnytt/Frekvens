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
            Serial.printf("%s: listening at port 6454\n", name);
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
        Serial.printf("%s: : unsupported package received\n", "Art-Net");
#endif
    }
}

void ArtNetMode::sleep()
{
    udp.reset();
}
