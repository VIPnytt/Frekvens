#include "config/constants.h"
#include "modes/ArtNetMode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void ArtNetMode::wake()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(6454))
    {
        udp->onPacket(&onPacket);
#ifdef F_DEBUG
        Serial.printf("%s: listening at %s:6454\n", name, Connectivity.domain.data());
#endif
    }
}

void ArtNetMode::onPacket(AsyncUDPPacket packet)
{
    if (packet.length() == 18 + COLUMNS * ROWS)
    {
        Display.setFrame(packet.data() + 18);
    }
#ifdef F_DEBUG
    else
    {
        Serial.printf("%s: : malformed packet received\n", _name.data());
    }
#endif
}

void ArtNetMode::sleep()
{
    udp.reset();
}
