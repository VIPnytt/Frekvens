#include "fonts/MiniFont.h"
#include "handlers/BitmapHandler.h"
#include "handlers/TextHandler.h"
#include "handlers/WeatherHandler.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void WeatherHandler::parse(const std::string code, const std::vector<Codeset> codesets)
{
    for (const WeatherHandler::Codeset codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            setSign(codeset.condition);
            return;
        }
    }
#ifdef F_VERBOSE
    Serial.print("WeatherHandler: unknown condition code ");
    Serial.println(code.c_str());
#endif
}

void WeatherHandler::parse(const uint8_t code, const std::vector<Codeset8> codesets)
{
    for (const WeatherHandler::Codeset8 codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            setSign(codeset.condition);
            return;
        }
    }
#ifdef F_VERBOSE
    Serial.print("WeatherHandler: unknown condition code ");
    Serial.println(code);
#endif
}

void WeatherHandler::parse(const uint16_t code, const std::vector<Codeset16> codesets)
{
    for (const WeatherHandler::Codeset16 codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            setSign(codeset.condition);
            return;
        }
    }
#ifdef F_VERBOSE
    Serial.print("WeatherHandler: unknown condition code ");
    Serial.println(code);
#endif
}

void WeatherHandler::setSign(Conditions condition)
{
    switch (condition)
    {
    case Conditions::CLEAR:
#if CELL_WIDTH == CELL_HEIGHT
        sign = conditionClear; // Square
#else
        sign = Display.getCellRatio() > 1 ? conditionClearTall : conditionClearWide; // Rectangular
#endif
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
    TextHandler text = TextHandler((String)temperature + "°", FontMini);
    BitmapHandler bitmap = BitmapHandler(sign);

    uint8_t textHeight = text.getHeight();
    uint8_t marginsY = max(0, ROWS - bitmap.getHeight() - textHeight) / 3;

    Display.clear();
    bitmap.draw((COLUMNS - bitmap.getWidth()) / 2, marginsY);
    text.draw((COLUMNS - text.getWidth()) / 2, ROWS - marginsY - textHeight);
}
