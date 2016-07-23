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
#include <stdlib.h>

using namespace std;

map<colorid_t, char*> COLOR_NAME_MAP;

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_Color() {
    //! カラー名のマッピング
    COLOR_NAME_MAP[COLOR_NONE] = "NONE";
    COLOR_NAME_MAP[COLOR_BLACK] = "BLACK";
    COLOR_NAME_MAP[COLOR_BLUE] = "BLUE";
    COLOR_NAME_MAP[COLOR_GREEN] = "GREEN";
    COLOR_NAME_MAP[COLOR_YELLOW] = "YELLOW";
    COLOR_NAME_MAP[COLOR_RED] = "RED";
    COLOR_NAME_MAP[COLOR_WHITE] = "WHITE";
    COLOR_NAME_MAP[COLOR_BROWN] = "BROWN";
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
        if (ev3_touch_sensor_is_pressed(touch_sensor)) {
            break;
        }
    }
       
    //! カラー名の取得
    colorid_t colorName = getColorName();
    
    //! RGB値を取得
    rgb_raw_t colorRGB = getColorRGBraw();
    
    //! 取得したカラー名をLCDに表示させる
    memset(message, '\0', sizeof(message));
    sprintf(message, "%s", COLOR_NAME_MAP[colorName]);
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
*/
colorid_t getColorName()
{
    return ev3_color_sensor_get_color(color_sensor);
}

/**
 * @brief カラーセンサーRGB値を取得
 *
 * @return  なし
*/
rgb_raw_t getColorRGBraw()
{
    rgb_raw_t colorRGB;
    ev3_color_sensor_get_rgb_raw(color_sensor, &colorRGB);   
    return colorRGB;
}
