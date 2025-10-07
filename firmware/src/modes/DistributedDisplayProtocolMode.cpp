#include "config/constants.h"
#include "modes/DistributedDisplayProtocolMode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void DistributedDisplayProtocolMode::wake()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(4048))
    {
        udp->onPacket(&onPacket);
#ifdef F_DEBUG
        Serial.printf("%s: listening at %s:4048\n", name, Connectivity.domain.data());
#endif
    }
}

void DistributedDisplayProtocolMode::onPacket(AsyncUDPPacket packet)
{
    const size_t len = packet.length();
    if (len == 10 + COLUMNS * ROWS || len == 14 + COLUMNS * ROWS)
    {
        Display.setFrame(packet.data() + len - COLUMNS * ROWS);
    }
#ifdef F_DEBUG
    else
    {
        Serial.printf("%s: malformed packet received\n", _name.data());
    }
#endif
}

void DistributedDisplayProtocolMode::sleep()
{
    udp.reset();
}
