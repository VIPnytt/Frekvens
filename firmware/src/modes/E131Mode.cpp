#if MODE_E131

#include <ESPmDNS.h>

#include "config/constants.h"
#include "modes/E131Mode.h"
#include "services/ConnectivityService.h"
#include "services/DisplayService.h"

void E131Mode::begin()
{
    udp = std::make_unique<AsyncUDP>();
    if (udp->listen(5568))
    {
        udp->onPacket(&onPacket);
        ESP_LOGD(name, "listening at " HOSTNAME ".local:5568");
    }
}

void E131Mode::onPacket(AsyncUDPPacket packet)
{
    if (packet.length() == 126 + GRID_COLUMNS * GRID_ROWS)
    {
        Display.setFrame(packet.data() + 126);
    }
}

void E131Mode::end()
{
    udp.reset();
}

#endif // MODE_E131
