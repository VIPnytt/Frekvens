#include "handlers/WeatherHandler.h"

#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/BitmapHandler.h"
#include "handlers/TextHandler.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

WeatherHandler::WeatherHandler() = default;

void WeatherHandler::parse(std::string code, std::vector<Codeset> codesets)
{
    for (const WeatherHandler::Codeset codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            setSign(codeset.condition);
            return;
        }
    }
    ESP_LOGD(_name.data(), "unknown condition code %s", code.c_str());
}

void WeatherHandler::parse(uint8_t code, std::vector<Codeset8> codesets)
{
    for (const WeatherHandler::Codeset8 codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            setSign(codeset.condition);
            return;
        }
    }
    ESP_LOGD(_name.data(), "unknown condition code %d", code);
}

void WeatherHandler::parse(uint16_t code, std::vector<Codeset16> codesets)
{
    for (const WeatherHandler::Codeset16 codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            setSign(codeset.condition);
            return;
        }
    }
    ESP_LOGD(_name.data(), "unknown condition code %d", code);
}

void WeatherHandler::setSign(Conditions condition)
{
    switch (condition)
    {
    case Conditions::CLEAR:
#if PITCH_HORIZONTAL == PITCH_VERTICAL
        sign = conditionClear;
#else
        sign = Display.getRatio() > 1.0f ? conditionClearTall : conditionClearWide;
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
        return;
    case Conditions::CLOUDY:
        sign = conditionCloudy;
        return;
    case Conditions::CLOUDY_PARTLY:
        sign = conditionCloudyPartly;
        return;
    case Conditions::EXCEPTION:
        sign = conditionExceptional;
        return;
    case Conditions::FOG:
        sign = conditionFog;
        return;
    case Conditions::RAIN:
        sign = conditionRain;
        return;
    case Conditions::SNOW:
        sign = conditionSnow;
        return;
    case Conditions::THUNDER:
        sign = conditionThunder;
        return;
    case Conditions::WIND:
        sign = conditionWind;
        return;
    }
}

void WeatherHandler::draw()
{
    TextHandler text = TextHandler(std::to_string(temperature) + "°", FontMini);
    BitmapHandler bitmap = BitmapHandler(sign);

    const uint8_t textHeight = text.getHeight();
    const uint8_t marginsY = max(0, GRID_ROWS - bitmap.getHeight() - textHeight) / 3;

    Display.clearFrame();
    bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, marginsY);
    text.draw((GRID_COLUMNS - text.getWidth()) / 2, GRID_ROWS - marginsY - textHeight);
}
