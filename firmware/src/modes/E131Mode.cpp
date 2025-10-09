#include "config/constants.h"

#if MODE_E131

#include "modes/E131Mode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void E131Mode::wake()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(5568))
    {
        udp->onPacket(&onPacket);
#ifdef F_DEBUG
        Serial.printf("%s: listening at %s:5568\n", name, Connectivity.domain.data());
#endif
    }
}

void E131Mode::onPacket(AsyncUDPPacket packet)
{
    if (packet.length() == 126 + COLUMNS * ROWS)
    {
        Display.setFrame(packet.data() + 126);
    }
#ifdef F_DEBUG
    else
    {
        Serial.printf("%s: : malformed packet received\n", _name.data());
    }
#endif
}

void E131Mode::sleep()
{
    udp.reset();
}

#endif // MODE_E131
