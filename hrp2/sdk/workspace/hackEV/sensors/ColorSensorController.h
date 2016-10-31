/**
 * @file    ColorSensorController.h
 * @brief   カラーセンサーを制御する
 */
#pragma once

#include "product.h"
#include <map>

//! \addtogroup 色識別モード
//@{
typedef enum {
    //! 黒 / 白
    COLOR_MODE_BLACK_WHITE = 0,

    //! 黒 / 灰 / 白
    COLOR_MODE_BLACK_GRAY_WHITE,

    //! カラー
    COLOR_MODE_COLOR,

    //! 色識別モードの数
    COLOR_MODE_COUNT
} COLOR_MODE;
//@}


const uint8_t COLOR_GRAY = (uint8_t)TNUM_COLOR + 1;

class ColorSensorController
{
public:
    explicit ColorSensorController(sensor_port_t _port);
    virtual void initialize();
    virtual uint8_t getColorID(COLOR_MODE modeColor = COLOR_MODE_BLACK_WHITE);
    virtual uint8_t getColorID(rgb_raw_t *colorRGB, COLOR_MODE modeColor = COLOR_MODE_BLACK_WHITE);
    virtual std::string getColorName(rgb_raw_t *colorRGB, COLOR_MODE modeColor = COLOR_MODE_BLACK_WHITE);
    virtual std::string getColorName(COLOR_MODE modeColor = COLOR_MODE_BLACK_WHITE);
    virtual std::string getColorNameByID(uint8_t color_id);
    virtual rgb_raw_t getColorRGBraw();
    virtual void correctColor(rgb_raw_t *colorRGB, double *redCorrected, double *greenCorrected, double *blueCorrected);
    virtual double getBrightness(double red, double green, double blue);
    virtual double getBrightness(rgb_raw_t *colorRGB);

private:
    void addColorMap(uint8_t color_id, std::string color_name);
    double getHue(double red, double green, double blue);

    //! 色情報
    std::map<uint8_t, std::string>  COLOR_NAME_MAP;

    //! カラーセンサーのポート
    sensor_port_t   port;

    //! \addtogroup 色の閾値
    //@{
    //! 灰(最小:仮決め)
    double BORDER_GRAY_MIN;

    //! 灰(最大:仮決め)
    double BORDER_GRAY_MAX;

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

    //! 緑と青(RGBの青で判別する)
    double BORDER_GREEN_BLUE_AS_BLUE;

        //! 緑と灰(RGBの赤で判別する)
    double BORDER_GREEN_GRAY_AS_RED;
    //@}
};
