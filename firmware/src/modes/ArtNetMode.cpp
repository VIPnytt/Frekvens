#include "config/constants.h"

#if MODE_ARTNET

#include "modes/ArtNetMode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void ArtNetMode::wake()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(6454))
    {
        udp->onPacket(&onPacket);
        ESP_LOGD(name, "listening at " HOSTNAME ".local:6454");
    }
}

void ArtNetMode::onPacket(AsyncUDPPacket packet)
{
    if (packet.length() == 18 + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(packet.data() + 18);
    }
    else
    {
        ESP_LOGV(_name.data(), "malformed packet received");
    }
}

void ArtNetMode::sleep()
{
    udp.reset();
}

#endif // MODE_ARTNET
