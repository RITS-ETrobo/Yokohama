/**
 * @file    ColorSensorController.cpp
 * @brief   カラーセンサーを制御する
 */
#include "ColorSensorController.h"
#include "user_function.h"

ColorSensorController::ColorSensorController(sensor_port_t _port)
    : port(_port)
    , BORDER_WHITE_MIN(100.0F)
    , BORDER_BLACK_MAX(30.0F)
    , BORDER_RED_YELLOW(0.08333F)
    , BORDER_YELLOW_GREEN(0.25000F)
    , BORDER_GREEN_BLUE(0.50000F)
    , BORDER_BLUE_RED(0.83333F)
{
}

/**
 * @brief   色情報を登録
 * 
 * @return  なし
*/
void ColorSensorController::addColorMap(colorid_t color_id, std::string color_name)
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
}

/**
 * @brief カラーセンサーで検知した色のIDを取得
 *
 * カラー名判定の種類 ： http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html#gaf11750614f023e665f98eca0b1f79c2f
 * @return  取得したカラーのID
 *          issues/811にて、RED, GREEN, BLUE, YELLOW, BLACK, WHITE のいずれかを返すように変更する。
*/
colorid_t ColorSensorController::getColorID()
{
    //'HSV色空間'について、wikipediaの項目を参考にした
    rgb_raw_t colorRGB = getColorRGBraw();

    double  maximumValue = getMaximumValue(colorRGB.r, colorRGB.g, colorRGB.b);
    double  minimumValue = getMinimumValue(colorRGB.r, colorRGB.g, colorRGB.b);

    //白黒判定
    if (BORDER_WHITE_MIN < minimumValue) { //白判定
        return  COLOR_WHITE;
    }

    if (maximumValue < BORDER_BLACK_MAX) { //黒判定
        return  COLOR_BLACK;
    }

    //色判定
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    correctColor(&colorRGB, &red, &green, &blue);

    //色相値計算
    double hue = getHue(red, green, blue);

    //色相値から色判定
    if (hue < BORDER_RED_YELLOW || BORDER_BLUE_RED <= hue) {
        return  COLOR_RED;
    }

    if (hue < BORDER_YELLOW_GREEN) {
        return  COLOR_YELLOW;
    }

    if (hue < BORDER_GREEN_BLUE) {
        return  COLOR_GREEN;
    }

    if (hue < BORDER_BLUE_RED) {
        return  COLOR_BLUE;
    }

    return  COLOR_NONE;
}

/**
 * @brief   色相値を求める
 * @param   red RGBでの赤の値
 * @param   green   RGBでの緑の値
 * @param   blue    RGBでの青の値
 * @return  色相値
*/
double ColorSensorController::getHue(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t max = getMaximumValue(red, green, blue);
    uint8_t min = getMinimumValue(red, green, blue);
    double  hue = (float)(max - min);
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
 * @return  取得した色の名前
*/
std::string ColorSensorController::getColorName()
{
    colorid_t   color = getColorID();
    return  getColorName(color);
}

/**
 * @brief   カラーセンサーで検知した色の名前を取得する
 * @param   color_id    色のID
 * @return  取得した色の名前
*/
std::string ColorSensorController::getColorName(colorid_t color_id)
{
    return  COLOR_NAME_MAP[color_id];
}

/**
 * @brief   カラーセンサーRGB値を取得
 * @return  カラーセンサーRGB値
*/
rgb_raw_t ColorSensorController::getColorRGBraw()
{
    rgb_raw_t colorRGB;
    ev3_color_sensor_get_rgb_raw(port, &colorRGB);
    return colorRGB;
}

/**
 * @brief   255超えの数値を255までに収める
 * @param   colorRGB    カラーセンサーから読み取った値
 * @param   red RGBでの赤の値
 * @param   green   RGBでの緑の値
 * @param   blue    RGBでの青の値
 * @return  輝度
*/
void ColorSensorController::correctColor(rgb_raw_t *colorRGB, uint8_t *redCorrected, uint8_t *greenCorrected, uint8_t *blueCorrected)
{
    *redCorrected = (uint8_t)((colorRGB->r > 255.0f) ? 1.0f : (colorRGB->r / 255.0f)); // = colorRGB->r / (double)maxInt;
    *greenCorrected = (uint8_t)((colorRGB->g > 255.0f) ? 1.0f : (colorRGB->g / 255.0f)); // = colorRGB->g / (double)maxInt;
    *blueCorrected = (uint8_t)((colorRGB->b > 255.0f) ? 1.0f : (colorRGB->b / 255.0f)); // = colorRGB->b / (double)maxInt;
}
