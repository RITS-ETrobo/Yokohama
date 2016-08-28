/**
 * @file    ColorBase.h
 * @brief   カラーセンサーを使って色を取得するクラスの基底クラス
 */
#include "ColorBase.h"

/**
 * @brief  コンストラクタ
*/
ColorBase::ColorBase()
{
}

/**
 *  @brief  初期化。色名のマッピングをおこなう
*/
void ColorBase::initialize()
{
    //! カラー名のマッピング
    COLOR_NAME_MAP[COLOR_NONE] = "NONE";
    COLOR_NAME_MAP[COLOR_BLACK] = "BLACK";
    COLOR_NAME_MAP[COLOR_BLUE] = "BLUE";
    COLOR_NAME_MAP[COLOR_GREEN] = "GREEN";
    COLOR_NAME_MAP[COLOR_YELLOW] = "YELLOW";
    COLOR_NAME_MAP[COLOR_RED] = "RED";
    COLOR_NAME_MAP[COLOR_WHITE] = "WHITE";
    COLOR_NAME_MAP[COLOR_BROWN] = "BROWN";
    COLOR_NAME_MAP[COLOR_GRAY] = "GRAY";
}

/**
 * @brief  カラーを白黒に変換した時の値を取得する
 * @param  color    カラーでの色の値
 * @return 白黒での値(0-100)
*/
uint8_t ColorBase::getRGB_BW(rgb_raw_t color)
{
    if ((color.r & 0xFF00) > 1) {
        color.r = 0xFF;
    }

    if ((color.g & 0xFF00) > 1) {
        color.g = 0xFF;
    }

    if ((color.b & 0xFF00) > 1) {
        color.b = 0xFF;
    }

    return  (uint8_t)(getLuminance(color) * 100 / (float)255);
}

/**
 * @brief  カラーを白黒に変換した時のIDを取得する
 * @param  color    カラーでの色の値
 * @return 色のID(白/灰/黒)
*/
uint8_t ColorBase::getID_BW(rgb_raw_t color)
{
    uint8_t color_bw = getRGB_BW(color);
}

/**
 * @brief  白黒の値からIDを取得する
 * @param  color    白黒での値
 * @return 色のID(白/灰/黒)
*/
uint8_t ColorBase::getID_BW(uint8_t colorBW)
{
    uint8_t color_bw = getID_BW(colorBW);
}

/**
 * @brief  色の名前を取得する(白/灰/黒)
 * @return 色の名前(白/灰/黒)
*/
char* ColorBase::getName_BW(rgb_raw_t color)
{
    return  COLOR_NAME_MAP[getID_BW(color)];
}

/**
 * @brief  色の名前を取得する(白/灰/黒)
 * @return 色の名前(白/灰/黒)
*/
char* ColorBase::getName_BW(uint8_t colorBW)
{
    return  COLOR_NAME_MAP[getID_BW(colorBW)];
}

/**
 * @brief  カラーを白黒に変更する
 * @param  color    カラーでの色の値
 * @return 輝度(0-255)
*/
uint8_t ColorBase::getLuminance(rgb_raw_t color)
{
    return  (uint8_t)(0.299 * (color.r & 0xFF) + 0.587 * (color.g & 0xFF) + 0.114 * (color.b & 0xFF));
}

/**
 * @brief  白黒の値から白黒のIDを取得する
 * @param  colorBW  白黒の値
 * @return 色ID(白/黒/灰)
*/
uint8_t ColorBase::getBWID(uint8_t colorBW)
{
    if (colorBW <= BW_VALUE_MAP[COLOR_BLACK] + 10) {
        return  COLOR_BLACK;
    }

    if (colorBW <= BW_VALUE_MAP[COLOR_WHITE] - 30) {
        return  COLOR_WHITE;
    }

    return  COLOR_GRAY;
}
