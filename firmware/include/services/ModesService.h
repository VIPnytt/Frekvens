#pragma once

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
#include "modes/GoogleWeatherMode.h"
#include "modes/HomeAssistantWeatherMode.h"
#include "modes/HomeThermometerMode.h"
#include "modes/JaggedWaveformMode.h"
#include "modes/LeafFallMode.h"
#include "modes/LinesMode.h"
#include "modes/MetaballsMode.h"
#include "modes/NoiseMode.h"
#include "modes/OpenMeteoMode.h"
#include "modes/OpenWeatherMode.h"
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
#include "modes/WorldWeatherOnlineMode.h"
#include "modes/WttrInMode.h"
#include "modes/YrMode.h"
#include "modules/ModeModule.h"
#include "modules/ServiceModule.h"

#include <vector>

class ModesService final : public ServiceModule
{
private:
    explicit ModesService() : ServiceModule("Modes") {};

    std::vector<ModeModule *> modes = {
#if MODE_ANIMATION
        new AnimationMode(),
#endif
#if MODE_ARROW
        new ArrowMode(),
#endif
#if MODE_BINARYCLOCK
        new BinaryClockMode(),
#endif
#if MODE_BINARYEPOCH
        new BinaryEpochMode(),
#endif
#if MODE_BLINDS
        new BlindsMode(),
#endif
#if MODE_BLINK
        new BlinkMode(),
#endif
#if MODE_BREAKOUTCLOCK
        new BreakoutClockMode(),
#endif
#if MODE_BRIGHT
        new BrightMode(),
#endif
#if MODE_CIRCLE
        new CircleMode(),
#endif
#if MODE_CLOCK
        new ClockMode(),
#endif
#if MODE_COUNTDOWN
        new CountdownMode(),
#endif
#if MODE_DRAW
        new DrawMode(),
#endif
#if MODE_EQUALIZER
        new EqualizerMode(),
#endif
#if MODE_FIREWORK
        new FireworkMode(),
#endif
#if MODE_FLIES
        new FliesMode(),
#endif
#if MODE_GAMEOFLIFE
        new GameOfLifeMode(),
#endif
#if MODE_GLITTER
        new GlitterMode(),
#endif
#if MODE_GOOGLEWEATHER
        new GoogleWeatherMode(),
#endif
#if MODE_HOMEASSISTANTWEATHER
        new HomeAssistantWeatherMode(),
#endif
#if MODE_HOMETHERMOMETER
        new HomeThermometerMode(),
#endif
#if MODE_JAGGEDWAVEFORM
        new JaggedWaveformMode(),
#endif
#if MODE_LEAFFALL
        new LeafFallMode(),
#endif
#if MODE_LINES
        new LinesMode(),
#endif
#if MODE_METABALLS
        new MetaballsMode(),
#endif
#if MODE_NOISE
        new NoiseMode(),
#endif
#if MODE_OPENMETEO
        new OpenMeteoMode(),
#endif
#if MODE_OPENWEATHER
        new OpenWeatherMode(),
#endif
#if MODE_PINGPONG
        new PingPongMode(),
#endif
#if MODE_PIXELSEQUENCE
        new PixelSequenceMode(),
#endif
#if MODE_RAIN
        new RainMode(),
#endif
#if MODE_RING
        new RingMode(),
#endif
#if MODE_SCAN
        new ScanMode(),
#endif
#if MODE_SMOOTHWAVEFORM
        new SmoothWaveformMode(),
#endif
#if MODE_SNAKE
        new SnakeMode(),
#endif
#if MODE_STARS
        new StarsMode(),
#endif
#if MODE_STREAM
        new StreamMode(),
#endif
#if MODE_TICKER
        new TickerMode(),
#endif
#if MODE_WAVEFORM
        new WaveformMode(),
#endif
#if MODE_WORLDWEATHERONLINE
        new WorldWeatherOnlineMode(),
#endif
#if MODE_WTTRIN
        new WttrInMode(),
#endif
#if MODE_YR
        new YrMode(),
#endif
    };

    unsigned long lastMillis = 0;

    ModeModule *mode = nullptr;

    TaskHandle_t taskHandle = nullptr;

    ModeModule *scheduled = nullptr;

    void setMode(ModeModule *mode, bool power = true);
    void transmit();

    static void onTask(void *parameter = nullptr);

public:
    static constexpr uint16_t stackSize = 1 << 13; // 8 kB

    void configure();
    void begin();
    void handle();
    void setActive(bool active);
    void setMode(const char *name);
    void setModeNext();
    void setModePrevious();
    [[nodiscard]] TaskHandle_t getTaskHandle() const;
    [[nodiscard]] const std::vector<ModeModule *> &getAll() const;
    void onReceive(JsonObjectConst payload, const char *source) override;

    static ModesService &getInstance();
};

extern ModesService &Modes;
