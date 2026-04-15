#pragma once

// NOLINTBEGIN(misc-include-cleaner)
#include "modes/AnimationMode.h"
#include "modes/ArrowMode.h"
#include "modes/BinaryClockMode.h"
#include "modes/BinaryEpochMode.h"
#include "modes/BlindsMode.h"
#include "modes/BlinkMode.h"
#include "modes/BreakoutClockMode.h"
#include "modes/BrightMode.h"
#include "modes/CircleMode.h"
#include "modes/ClockMode.h"
#include "modes/CountdownMode.h"
#include "modes/DrawMode.h"
#include "modes/EqualizerMode.h"
#include "modes/FireworkMode.h"
#include "modes/FliesMode.h"
#include "modes/GameOfLifeMode.h"
#include "modes/GlitterMode.h"
#include "modes/HomeThermometerMode.h"
#include "modes/JaggedWaveformMode.h"
#include "modes/LeafFallMode.h"
#include "modes/LinesMode.h"
#include "modes/MetaballsMode.h"
#include "modes/NoiseMode.h"
#include "modes/PingPongMode.h"
#include "modes/PixelSequenceMode.h"
#include "modes/RainMode.h"
#include "modes/RingMode.h"
#include "modes/ScanMode.h"
#include "modes/SmoothWaveformMode.h"
#include "modes/SnakeMode.h"
#include "modes/StarsMode.h"
#include "modes/StreamMode.h"
#include "modes/TickerMode.h"
#include "modes/WaveformMode.h"
#include "modes/WeatherMode.h"
#include "modules/ModeModule.h"
#include "modules/ServiceModule.h"
// NOLINTEND(misc-include-cleaner)

#include <array>
#include <vector>

class ModesService final : public ServiceModule
{
private:
    explicit ModesService() : ServiceModule("Modes") {};

    bool scheduled = false;

    unsigned long lastMillis = 0;

    std::unique_ptr<ModeModule> mode{};

    TaskHandle_t taskHandle{};

    void transmit();

    static void onTask(void *parameter = nullptr);

public:
    static constexpr uint16_t stackSize = 1U << 13U; // 8 kB

    static constexpr auto names = std::to_array<std::string_view>({
#if MODE_ANIMATION
        AnimationMode::name,
#endif
#if MODE_ARROW
        ArrowMode::name,
#endif
#if MODE_BINARYCLOCK
        BinaryClockMode::name,
#endif
#if MODE_BINARYEPOCH
        BinaryEpochMode::name,
#endif
#if MODE_BLINDS
        BlindsMode::name,
#endif
#if MODE_BLINK
        BlinkMode::name,
#endif
#if MODE_BREAKOUTCLOCK
        BreakoutClockMode::name,
#endif
#if MODE_BRIGHT
        BrightMode::name,
#endif
#if MODE_CIRCLE
        CircleMode::name,
#endif
#if MODE_CLOCK
        ClockMode::name,
#endif
#if MODE_COUNTDOWN
        CountdownMode::name,
#endif
#if MODE_DRAW
        DrawMode::name,
#endif
#if MODE_EQUALIZER
        EqualizerMode::name,
#endif
#if MODE_FIREWORK
        FireworkMode::name,
#endif
#if MODE_FLIES
        FliesMode::name,
#endif
#if MODE_GAMEOFLIFE
        GameOfLifeMode::name,
#endif
#if MODE_GLITTER
        GlitterMode::name,
#endif
#if MODE_HOMETHERMOMETER
        HomeThermometerMode::name,
#endif
#if MODE_JAGGEDWAVEFORM
        JaggedWaveformMode::name,
#endif
#if MODE_LEAFFALL
        LeafFallMode::name,
#endif
#if MODE_LINES
        LinesMode::name,
#endif
#if MODE_METABALLS
        MetaballsMode::name,
#endif
#if MODE_NOISE
        NoiseMode::name,
#endif
#if MODE_PINGPONG
        PingPongMode::name,
#endif
#if MODE_PIXELSEQUENCE
        PixelSequenceMode::name,
#endif
#if MODE_RAIN
        RainMode::name,
#endif
#if MODE_RING
        RingMode::name,
#endif
#if MODE_SCAN
        ScanMode::name,
#endif
#if MODE_SMOOTHWAVEFORM
        SmoothWaveformMode::name,
#endif
#if MODE_SNAKE
        SnakeMode::name,
#endif
#if MODE_STARS
        StarsMode::name,
#endif
#if MODE_STREAM
        StreamMode::name,
#endif
#if MODE_TICKER
        TickerMode::name,
#endif
#if MODE_WAVEFORM
        WaveformMode::name,
#endif
#if MODE_WEATHER
        WeatherMode::name,
#endif
    });

    void configure();
    void begin();
    void handle();
    void setActive(bool active);
    [[nodiscard]] ModeModule *getMode();
    [[nodiscard]] std::unique_ptr<ModeModule> getMode(std::string_view modeName);
    void setMode(std::string_view modeName, bool power = true);
    void setModeNext();
    void setModePrevious();
    [[nodiscard]] TaskHandle_t getTaskHandle() const;
    void onReceive(JsonObjectConst payload, std::string_view source) override;

    static ModesService &getInstance();
};

extern ModesService &Modes; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
