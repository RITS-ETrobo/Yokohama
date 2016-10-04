/**
 * @file    ColorSensorController.h
 * @brief   カラーセンサーを制御する
 */
#pragma once

#include "product.h"
#include <map>

//! ターゲット依存の定義
#include "target_test.h"

class ColorSensorController
{
public:
    explicit ColorSensorController(sensor_port_t _port);
    virtual void initialize();
    virtual colorid_t getColorID();
    virtual std::string getColorName();
    virtual std::string getColorName(colorid_t color);
    virtual rgb_raw_t getColorRGBraw();
    virtual void correctColor(rgb_raw_t *colorRGB, uint8_t *redCorrected, uint8_t *greenCorrected, uint8_t *blueCorrected);
    virtual uint8_t getBrightness(uint8_t red, uint8_t green, uint8_t blue);
    virtual uint8_t getBrightness(rgb_raw_t *colorRGB);

private:
    void addColorMap(colorid_t color_id, std::string color_name);
    double getHue(uint8_t red, uint8_t green, uint8_t blue);

    //! 色情報
    std::map<colorid_t, std::string>    COLOR_NAME_MAP;

    //! カラーセンサーのポート
    sensor_port_t   port;

    //! \addtogroup 色の閾値
    //@{
    //! 白(仮決め)
    double BORDER_WHITE_MIN;

    //! 黒(仮決め)
    double BORDER_BLACK_MAX;

    //! 赤/黄  (0.0f + 1.0f/6.0f) / 2.0f;
    double BORDER_RED_YELLOW;

    //! 黄/緑  (1.0f/6.0f + 2.0f/6.0f) / 2.0f
    double BORDER_YELLOW_GREEN;

    //! 緑/青  (2.0f/6.0f + 4.0f/6.0f) / 2.0f;
    double BORDER_GREEN_BLUE;

    //! 青/赤  (4.0f/6.0f + 1.0f) / 2.0f;
    double BORDER_BLUE_RED;
    //@}
};
