/**
 * @file    ColorSensorController.cpp
 * @brief   カラーセンサーを制御する
 */
#include <string.h>
#include "ColorSensorController.h"
#include "user_function.h"

ColorSensorController::ColorSensorController(sensor_port_t _port)
    : port(_port)
    , BORDER_GRAY_MIN(0.1 * 255)
    , BORDER_GRAY_MAX(0.6 * 255)
    , BORDER_WHITE_MIN(100.0F)
    , BORDER_BLACK_MAX(30.0F)
    , BORDER_RED_YELLOW(0.08333F)
    , BORDER_YELLOW_GREEN(0.25000F)
    , BORDER_GREEN_BLUE(0.50000F)
    , BORDER_BLUE_RED(0.83333F)
    , BORDER_GREEN_BLUE_AS_BLUE(70 / (double)255)
    , BORDER_GREEN_GRAY_AS_RED(50 / (double)255)
{
}

/**
 * @brief   色情報を登録
 * 
 * @return  なし
*/
void ColorSensorController::addColorMap(uint8_t color_id, std::string color_name)
{
    COLOR_NAME_MAP[color_id] = color_name;
}

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void ColorSensorController::initialize() {
    //! カラー名のマッピング
    addColorMap(COLOR_NONE, "NONE");
    addColorMap(COLOR_BLACK, "BLACK");
    addColorMap(COLOR_BLUE, "BLUE");
    addColorMap(COLOR_GREEN, "GREEN");
    addColorMap(COLOR_YELLOW, "YELLOW");
    addColorMap(COLOR_RED, "RED");
    addColorMap(COLOR_WHITE, "WHITE");
    addColorMap(COLOR_BROWN, "BROWN");
    addColorMap(COLOR_GRAY, "GRAY");
}

/**
 * @brief   カラーセンサーで検知した色のIDを取得
 * @param   modeColor   色識別モード
 *
 * カラー名判定の種類 ： http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html#gaf11750614f023e665f98eca0b1f79c2f
 * @return  取得したカラーのID(RED, GREEN, BLUE, YELLOW, BLACK, WHITE, GRAY, NONE のいずれか)
*/
uint8_t ColorSensorController::getColorID(COLOR_MODE modeColor /*= COLOR_MODE_BLACK_WHITE*/)
{
    //'HSV色空間'について、wikipediaの項目を参考にした
    rgb_raw_t colorRGB = getColorRGBraw();

    return  getColorID(&colorRGB, modeColor);
}

/**
 * @brief   カラーセンサーで検知した色のIDを取得
 * @param   colorRGB    カラーセンサーで取得した値
 * @param   modeColor   色識別モード
 *
 * カラー名判定の種類 ： http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html#gaf11750614f023e665f98eca0b1f79c2f
 * @return  取得したカラーのID(RED, GREEN, BLUE, YELLOW, BLACK, WHITE, GRAY, NONE のいずれか)
*/
uint8_t ColorSensorController::getColorID(rgb_raw_t *colorRGB, COLOR_MODE modeColor /*= COLOR_MODE_BLACK_WHITE*/)
{
    double  maximumValue = getMaximumValue(colorRGB->r, colorRGB->g, colorRGB->b);
    double  minimumValue = getMinimumValue(colorRGB->r, colorRGB->g, colorRGB->b);

    if ((modeColor == COLOR_MODE_BLACK_WHITE) || (modeColor == COLOR_MODE_BLACK_GRAY_WHITE)) {
        double  brightness = getBrightness(colorRGB);
        if (BORDER_GRAY_MIN > brightness) {
            return  (uint8_t)COLOR_BLACK;
        }

        if (brightness > BORDER_GRAY_MAX) {
            return  (uint8_t)COLOR_WHITE;
        }

        if (modeColor == COLOR_MODE_BLACK_GRAY_WHITE) {
            return  (uint8_t)COLOR_GRAY;
        }

        return  (uint8_t)COLOR_BLACK;
    }

    //白黒判定
    if (BORDER_WHITE_MIN < minimumValue) { //白判定
        return  (uint8_t)COLOR_WHITE;
    }

    if (maximumValue < BORDER_BLACK_MAX) { //黒判定
        return  (uint8_t)COLOR_BLACK;
    }

    //色判定
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;
    correctColor(colorRGB, &red, &green, &blue);

    //色相値計算
    double hue = getHue(red, green, blue);

    //色相値から色判定
    if (hue < BORDER_RED_YELLOW || BORDER_BLUE_RED <= hue) {
        return  (uint8_t)COLOR_RED;
    }

    if (hue < BORDER_YELLOW_GREEN) {
        return  (uint8_t)COLOR_YELLOW;
    }

    if (hue < BORDER_GREEN_BLUE) {
        if (red > BORDER_GREEN_GRAY_AS_RED) {
           return  (uint8_t)COLOR_GRAY;
        }

        if (blue > BORDER_GREEN_BLUE_AS_BLUE) {
           // BLUEはGREENと判定される事がある。その為、RGBのblueを比較する
           return  (uint8_t)COLOR_BLUE;
        }

        return  (uint8_t)COLOR_GREEN;
    }

    if (hue < BORDER_BLUE_RED) {
        return  (uint8_t)COLOR_BLUE;
    }

    return  (uint8_t)COLOR_NONE;
}

/**
 * @brief   色相値を求める
 * @param   red RGBでの赤の割合
 * @param   green   RGBでの緑の割合
 * @param   blue    RGBでの青の割合
 * @return  色相値
*/
double ColorSensorController::getHue(double red, double green, double blue)
{
    double max = getMaximumValue(red, green, blue);
    double min = getMinimumValue(red, green, blue);
    double  hue = max - min;
    if (hue > 0.0f) {
        if (red - 0.0001f < max && max < red + 0.0001f) {
            hue = (green - blue) / hue;
            if (hue < 0.0f) {
                //  色は最も赤い色に近いが、その中でも緑よりも青っぽかったら色相は6に近い値になる。
                //  色相は循環している為、必ずしも正の値にならなくても良い
                hue += 6.0f;
            }
        } else if (green - 0.0001f < max && max < green + 0.0001f) {
            hue = 2.0f + (blue - red) / hue;
        } else {
            hue = 4.0f + (red - green) / hue;
        }
    }

    hue /= 6.0f;
    return  hue;
}

/**
 * @brief   カラーセンサーで検知した色の名前を取得する
 * @param   colorRGB    カラーセンサーから読み取った値
 * @param   modeColor   色識別モード
 * @return  取得した色の名前
*/
std::string ColorSensorController::getColorName(rgb_raw_t *colorRGB, COLOR_MODE modeColor /*= COLOR_MODE_BLACK_WHITE*/)
{
    uint8_t color = getColorID(colorRGB, modeColor);
    return  getColorNameByID(color);
}

/**
 * @brief   カラーセンサーで検知した色の名前を取得する
 * @param   modeColor   色識別モード
 * @return  取得した色の名前
*/
std::string ColorSensorController::getColorName(COLOR_MODE modeColor /*= COLOR_MODE_BLACK_WHITE*/)
{
    uint8_t color = getColorID(modeColor);
    return  getColorNameByID(color);
}

/**
 * @brief   カラーセンサーで検知した色の名前を取得する
 * @param   color_id    色のID
 * @return  取得した色の名前
*/
std::string ColorSensorController::getColorNameByID(uint8_t color_id)
{
    return  COLOR_NAME_MAP[color_id];
}

/**
 * @brief   カラーセンサーRGB値を取得
 * @return  カラーセンサーRGB値
*/
rgb_raw_t ColorSensorController::getColorRGBraw()
{
    rgb_raw_t   colorRGB;
    memset(&colorRGB, '\0', sizeof(rgb_raw_t));

#ifndef EV3_UNITTEST
    ev3_color_sensor_get_rgb_raw(port, &colorRGB);
#endif  //  EV3_UNITTEST

    return  colorRGB;
}

/**
 * @brief   輝度を取得 http://www.dinop.com/vc/image_gray_scale.html
 * @param   red RGBでの赤の割合
 * @param   green   RGBでの緑の割合
 * @param   blue    RGBでの青の割合
 * @return  輝度
*/
double ColorSensorController::getBrightness(double red, double green, double blue)
{
    return  (double)(77 * red + 150 * green + 29 * blue);
}

/**
 * @brief   輝度を取得 http://www.dinop.com/vc/image_gray_scale.html
 * @param   colorRGB    カラーセンサーから読み取った値
 * @return  輝度
*/
double ColorSensorController::getBrightness(rgb_raw_t *colorRGB)
{
    double redCorrected = 0.0;
    double greenCorrected = 0.0;
    double blueCorrected = 0.0;

    correctColor(colorRGB, &redCorrected, &greenCorrected, &blueCorrected);
    return  getBrightness(redCorrected, greenCorrected, blueCorrected);
}
/**
 * @brief   255超えの数値を255まででの割合に収める
 * @param   colorRGB    カラーセンサーから読み取った値
 * @param   red RGBでの赤の割合
 * @param   green   RGBでの緑の割合
 * @param   blue    RGBでの青の割合
 * @return  輝度
*/
void ColorSensorController::correctColor(rgb_raw_t *colorRGB, double *redCorrected, double *greenCorrected, double *blueCorrected)
{
    *redCorrected = (colorRGB->r > 255.0f) ? 1.0f : (colorRGB->r / 255.0f); // = colorRGB->r / (double)maxInt;
    *greenCorrected = (colorRGB->g > 255.0f) ? 1.0f : (colorRGB->g / 255.0f); // = colorRGB->g / (double)maxInt;
    *blueCorrected = (colorRGB->b > 255.0f) ? 1.0f : (colorRGB->b / 255.0f); // = colorRGB->b / (double)maxInt;
}
