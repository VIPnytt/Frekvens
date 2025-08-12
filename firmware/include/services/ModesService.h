#pragma once

#include <ArduinoJson.h>
#include <vector>

#include "config/constants.h"
#include "modes/AnimationMode.h"
#include "modes/ArrowMode.h"
#include "modes/ArtNetMode.h"
#include "modes/BinaryClockMode.h"
#include "modes/BinaryEpochMode.h"
#include "modes/BlindsMode.h"
#include "modes/BlinkMode.h"
#include "modes/BoldClockMode.h"
#include "modes/BreakoutClockMode.h"
#include "modes/BrightMode.h"
#include "modes/CircleMode.h"
#include "modes/CountdownMode.h"
#include "modes/DistributedDisplayProtocolMode.h"
#include "modes/DrawMode.h"
#include "modes/E131Mode.h"
#include "modes/EqualizerMode.h"
#include "modes/FireworkMode.h"
#include "modes/FliesMode.h"
#include "modes/GameOfLifeClockMode.h"
#include "modes/GameOfLifeMode.h"
#include "modes/GlitterMode.h"
#include "modes/HomeAssistantWeatherMode.h"
#include "modes/HomeThermometerMode.h"
#include "modes/JaggedWaveformMode.h"
#include "modes/LeafFallMode.h"
#include "modes/LinesMode.h"
#include "modes/MetaballsMode.h"
#include "modes/NoiseMode.h"
#include "modes/OpenMetroMode.h"
#include "modes/OpenWeatherMode.h"
#include "modes/PingPongClockMode.h"
#include "modes/PingPongMode.h"
#include "modes/PixelSequenceMode.h"
#include "modes/RainMode.h"
#include "modes/RingMode.h"
#include "modes/ScanMode.h"
#include "modes/SmallClockMode.h"
#include "modes/SmoothWaveformMode.h"
#include "modes/SnakeClockMode.h"
#include "modes/SnakeMode.h"
#include "modes/StarsMode.h"
#include "modes/TickerMode.h"
#include "modes/TickingClockMode.h"
#include "modes/WaveformMode.h"
#include "modes/WorldWeatherOnlineMode.h"
#include "modes/WttrInMode.h"
#include "modes/YrMode.h"
#include "modules/ModeModule.h"
#include "modules/ServiceModule.h"

class ModesService : public ServiceModule
{
private:
    ModesService() : ServiceModule("Modes") {};

    const std::vector<ModeModule *> modules = {
#if MODE_ANIMATION
        new AnimationMode(),
#endif
#if MODE_ARROW
        new ArrowMode(),
#endif
#if MODE_ARTNET
        new ArtNetMode(),
#endif
#if MODE_BINARYCLOCK
        new BinaryClockMode(),
#endif
#if MODE_BINARYEPOCH
        new BinaryEpochMode(),
#endif
#if MODE_BLINK
        new BlinkMode(),
#endif
#if MODE_BLINDS
        new BlindsMode(),
#endif
#if MODE_BOLDCLOCK
        new BoldClockMode(),
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
#if MODE_COUNTDOWN
        new CountdownMode(),
#endif
#if MODE_DISTRIBUTEDDISPLAYPROTOCOL
        new DistributedDisplayProtocolMode(),
#endif
#if MODE_DRAW
        new DrawMode(),
#endif
#if MODE_E131
        new E131Mode(),
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
#if MODE_GAMEOFLIFECLOCK
        new GameOfLifeClockMode(),
#endif
#if MODE_GLITTER
        new GlitterMode(),
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
#if MODE_OPENMETRO
        new OpenMetroMode(),
#endif
#if MODE_OPENWEATHER
        new OpenWeatherMode(),
#endif
#if MODE_PINGPONG
        new PingPongMode(),
#endif
#if MODE_PINGPONGCLOCK
        new PingPongClockMode(),
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
#if MODE_SMALLCLOCK
        new SmallClockMode(),
#endif
#if MODE_SMOOTHWAVEFORM
        new SmoothWaveformMode(),
#endif
#if MODE_SNAKE
        new SnakeMode(),
#endif
#if MODE_SNAKECLOCK
        new SnakeClockMode(),
#endif
#if MODE_STARS
        new StarsMode(),
#endif
#if MODE_TICKER
        new TickerMode(),
#endif
#if MODE_TICKINGCLOCK
        new TickingClockMode(),
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

    bool pending = false;

    unsigned long lastMillis = 0;

#ifdef F_VERBOSE
    unsigned long _lastMillis = 0;
#endif

#ifdef TASK_STACK_MODES
    static constexpr uint16_t stackSize = TASK_STACK_MODES;
#else
    static constexpr uint16_t stackSize = 8192;
#endif

    void set(ModeModule *mode);
    void splash();
    void teardown();
    void transmit();

    static void onTask(void *parameter = nullptr);

public:
    TaskHandle_t taskHandle = nullptr;
    ModeModule *active = nullptr;

    void setup();
    void ready();
    void handle();
    void set(bool enable, const char *const source);
    void set(const char *const name);
    void next();
    void previous();
    const std::vector<ModeModule *> &getAll() const;
    void receiverHook(const JsonDocument doc) override;

    static ModesService &getInstance();
};

extern ModesService &Modes;
