#pragma once

#if EXTENSION_WEBAPP

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class WebAppExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Web app"};
    static constexpr std::string_view path{"/webapp/index.html.gz"};

    static inline size_t length{0U};

    static inline std::array<char, 11> etag{'\0'};

    static void onGetRoot(AsyncWebServerRequest *request);
    static void onHeadRoot(AsyncWebServerRequest *request);

public:
    explicit WebAppExtension() : ExtensionModule(name) {};

    void configure() override;
};

#endif // EXTENSION_WEBAPP
