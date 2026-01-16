#if MODE_STREAM

#include <ESPmDNS.h>

#include "config/constants.h"
#include "modes/StreamMode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void StreamMode::begin()
{
    udp = std::make_unique<AsyncUDP>();
    udp->listen(4048);
    udp->listen(5568);
    udp->listen(6454);
    udp->onPacket(&onPacket);
    ESP_LOGD(name, "Art-Net listening at " HOSTNAME ".local:6454");
    ESP_LOGD(name, "Distributed Display Protocol listening at " HOSTNAME ".local:4048");
    ESP_LOGD(name, "E1.31 listening at " HOSTNAME ".local:5568");
}

void StreamMode::onPacket(AsyncUDPPacket packet)
{
    const uint16_t port = packet.localPort();
    const size_t len = packet.length();
    if ((port == 4048 && (len == 10 + GRID_COLUMNS * GRID_ROWS || len == 14 + GRID_COLUMNS * GRID_ROWS)) ||
        (port == 6454 && len == 18 + GRID_COLUMNS * GRID_ROWS) ||
        (port == 5568 && len == 126 + GRID_COLUMNS * GRID_ROWS))
    {
        Display.setFrame(packet.data() + len - GRID_COLUMNS * GRID_ROWS);
    }
}

void StreamMode::end()
{
    udp.reset();
}

#endif // MODE_STREAM
