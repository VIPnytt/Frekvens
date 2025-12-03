#if MODE_DISTRIBUTEDDISPLAYPROTOCOL

#include <ESPmDNS.h>

#include "config/constants.h"
#include "modes/DistributedDisplayProtocolMode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void DistributedDisplayProtocolMode::begin()
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
}

void DistributedDisplayProtocolMode::end()
{
    udp.reset();
}

#endif // MODE_DISTRIBUTEDDISPLAYPROTOCOL
