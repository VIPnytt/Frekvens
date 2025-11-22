#include "config/constants.h"

#if MODE_DISTRIBUTEDDISPLAYPROTOCOL

#include "modes/DistributedDisplayProtocolMode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void DistributedDisplayProtocolMode::wake()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(4048))
    {
        udp->onPacket(&onPacket);
        ESP_LOGD(name, "listening at " HOSTNAME ".local:4048");
    }
}

void DistributedDisplayProtocolMode::onPacket(AsyncUDPPacket packet)
{
    const size_t len = packet.length();
    if (len == 10 + GRID_COLUMNS * GRID_ROWS || len == 14 + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(packet.data() + len - GRID_COLUMNS * GRID_ROWS);
    }
    else
    {
        ESP_LOGV(_name.data(), "malformed packet received");
    }
}

void DistributedDisplayProtocolMode::sleep()
{
    udp.reset();
}

#endif // MODE_DISTRIBUTEDDISPLAYPROTOCOL
