/**
 * @file    ColorBase.h
 * @brief   カラーセンサーを使って色を取得するクラスの基底クラス
 */
#include "ColorBase.h"

/**
    @brief  コンストラクタ
*/
ColorBase::ColorBase(sensor_port_t portColor_)
    : portColor(portColor_)
{
}

/**
    @brief  色の生の値を取得する(白黒)
    @return 色の生の値(白黒)
*/
uint32_t ColorBase::getRGB_BW()
{
}

/**
    @brief  色のIDを取得する(白/灰/黒)
    @return 色のID(白/灰/黒)
*/
uint8_t ColorBase::getID_BW()
{
}

/**
    @brief  色の名前を取得する(白/灰/黒)
    @return 色の名前(白/灰/黒)
*/
char* ColorBase::getName_BW()
{
}

/**
    @brief  カラーを白黒での値に変更する
@return 色の生の値(白黒)
*/
uint8 ColorBase::color2BW(uint32_t color)
{
}
