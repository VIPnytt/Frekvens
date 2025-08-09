#pragma once

#include <ArduinoJson.h>
#include <vector>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class DeviceService : public ServiceModule
{
private:
    DeviceService() : ServiceModule("Device") {};

#ifdef F_DEBUG
    static constexpr std::string_view
        reset_brownout = "Brownout",
        reset_exception = "Exception",
#if SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
        reset_external = "External",
#endif // SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
        reset_interrupt = "Interrupt watchdog",
        reset_power = "Power",
        reset_software = "Software",
        reset_task = "Task watchdog",
        reset_watchdog = "Watchdog";
#endif // F_DEBUG

    unsigned long lastMillis = 0;

#ifdef F_DEBUG
    const char *reset = nullptr;
#endif

    std::string latest = "";

    JsonDocument transmits;

    void ready();

    void transmit();
    void receiverHook(const JsonDocument doc) override;

public:
    const std::string_view repository = "https://github.com/VIPnytt/Frekvens";

    void init();
    void run();

    void identify();
    void power(bool state);
    void update();

    void transmit(JsonDocument doc, const char *const source, bool retain = true);
    void receive(const JsonDocument doc, const char *const source, const char *const destination);

    const std::vector<const char *> getNames() const;
    const JsonDocument getTransmits() const;

    static DeviceService &getInstance();
};

extern DeviceService &Device;
