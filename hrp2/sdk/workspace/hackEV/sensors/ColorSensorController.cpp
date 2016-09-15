/**
 * @file    ColorSensorController.cpp
 * @brief   カラーセンサーを制御する
 */

#include "ev3api.h"
#include "utilities.h"
#include "ColorSensorController.h"
#include "LCDController.h"
#include <stdio.h>
#include <map>
#include <string>
#include <stdlib.h>

std::map<colorid_t, std::string>    COLOR_NAME_MAP;

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_ColorMap(colorid_t color_id, std::string color_name)
{
    COLOR_NAME_MAP[color_id] = color_name;
}

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_Color() {
    //! カラー名のマッピング
    initialize_ColorMap(COLOR_NONE, "NONE");
    initialize_ColorMap(COLOR_BLACK, "BLACK");
    initialize_ColorMap(COLOR_BLUE, "BLUE");
    initialize_ColorMap(COLOR_GREEN, "GREEN");
    initialize_ColorMap(COLOR_YELLOW, "YELLOW");
    initialize_ColorMap(COLOR_RED, "RED");
    initialize_ColorMap(COLOR_WHITE, "WHITE");
    initialize_ColorMap(COLOR_BROWN, "BROWN");
}

/**
 * @brief   カラーのLCD表示
 * @return  なし
*/
void viewColor()
{
    //! 初期化（カラー名のマッピング）
    initialize_Color();
    
    ev3_speaker_play_tone(NOTE_F5, 200);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_E6, 200);
    char message[16];
    
    memset(message, '\0', sizeof(message));
    sprintf(message, " Push Touch Sensor!");
    writeStringLCD(message);
    
    //! タッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }
       
    //! カラー名の取得
    colorid_t colorName = getColorName();
    
    //! RGB値を取得
    rgb_raw_t colorRGB = getColorRGBraw();
    
    //! 取得したカラー名をLCDに表示させる
    memset(message, '\0', sizeof(message));
    sprintf(message, "%s", COLOR_NAME_MAP[colorName].c_str());
    writeStringLCD(message);
    
    //! 取得したRGB値をLCDに表示させる
    memset(message, '\0', sizeof(message));
    sprintf(message, "R: %d", colorRGB.r);
    writeStringLCD(message);

    memset(message, '\0', sizeof(message));
    sprintf(message, "G: %d", colorRGB.g);
    writeStringLCD(message);

    memset(message, '\0', sizeof(message));
    sprintf(message, "B: %d", colorRGB.b);
    writeStringLCD(message);
}

/**
 * @brief カラーセンサーで検知した色の名前を取得
 *
 * カラー名判定の種類：http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html#gaf11750614f023e665f98eca0b1f79c2f
 * @return  取得したカラーの名前
 *          issues/811にて、RED, GREEN, BLUE, YELLOW, BLACK, WHITE のいずれかを返すように変更する。
*/
colorid_t getColorName()
{
    //'HSV色空間'について、wikipediaの項目を参考にした
    colorid_t retColorId = COLOR_NONE;
    
    const double BORDER_WHITE_MIN_INT = 100; //仮決め
    const double BORDER_BLACK_MAX_INT = 30; //仮決め
    
    const double BORDER_RED_YELLOW = 0.08333f; //(0.0f + 1.0f/6.0f) / 2.0f;
    const double BORDER_YELLOW_GREEN = 0.25000f; //(1.0f/6.0f + 2.0f/6.0f) / 2.0f
    const double BORDER_GREEN_BLUE = 0.50000f; //(2.0f/6.0f + 4.0f/6.0f) / 2.0f;
    const double BORDER_BLUE_RED = 0.83333f; //(4.0f/6.0f + 1.0f) / 2.0f;
    
    rgb_raw_t colorRGB = getColorRGBraw();
    int maxInt, minInt;
    
    maxInt = (colorRGB.r > colorRGB.g) ? colorRGB.r: colorRGB.g;
    maxInt = (maxInt > colorRGB.b) ? maxInt: colorRGB.b;
    
    minInt = (colorRGB.r < colorRGB.g) ? colorRGB.r: colorRGB.g;
    minInt = (minInt < colorRGB.b) ? minInt: colorRGB.b;
    
    //判定開始
    if (BORDER_WHITE_MIN_INT < minInt) { //白判定
        retColorId = COLOR_WHITE;
    } else if (maxInt < BORDER_BLACK_MAX_INT) { //黒判定
        retColorId = COLOR_BLACK;
    } else {
        //色判定
        double r, g, b;
        double max, min;
        r = (colorRGB.r > 255.0f) ? 1.0f: (colorRGB.r / 255.0f); // = colorRGB.r / (double)maxInt;
        g = (colorRGB.g > 255.0f) ? 1.0f: (colorRGB.g / 255.0f); // = colorRGB.g / (double)maxInt;
        b = (colorRGB.b > 255.0f) ? 1.0f: (colorRGB.b / 255.0f); // = colorRGB.b / (double)maxInt;
        
        max = (r > g) ? r: g;
        max = (max > b) ? max: b;
        
        min = (r < g) ? r: g;
        min = (min < b) ? min: b;
        //色相値計算
        double hue = max - min;
        if (hue > 0.0f) {
            if (r-0.0001f < max && max < r+0.0001f) {
                hue = (g - b) / hue;
                if (hue < 0.0f) {
                    hue + 6.0f;
                }
            } else if (g-0.0001f < max && max <g+0.0001f) {
                hue = 2.0f + (b - r) / hue;
            } else {
                hue = 4.0f + (r - g) / hue;
            }
        }
        hue /= 6.0f;
        
        //色相値から色判定
        if (hue < BORDER_RED_YELLOW || BORDER_BLUE_RED <= hue) {
            retColorId = COLOR_RED;
        } else if (hue < BORDER_YELLOW_GREEN) {
            retColorId = COLOR_YELLOW;
        } else if (hue < BORDER_GREEN_BLUE) {
            retColorId = COLOR_GREEN;
        } else if (hue < BORDER_BLUE_RED) {
            retColorId = COLOR_BLUE;
        }
    }
    
    return retColorId;
    //return ev3_color_sensor_get_color(EV3_SENSOR_COLOR);
}

/**
 * @brief カラーセンサーRGB値を取得
 *
 * @return  なし
*/
rgb_raw_t getColorRGBraw()
{
    rgb_raw_t colorRGB;
    ev3_color_sensor_get_rgb_raw(EV3_SENSOR_COLOR, &colorRGB);   
    return colorRGB;
}
