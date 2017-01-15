/**
 * @file    user_function.cpp
 * @brief   imgビルドでエラーになる関数を自作した
 */
#include "user_function.h"

//! 円周率
const float Pi = 3.14159265359F;

/**
 *  @brief  imgビルドでfmodがエラーになる為、自作した
 *  @param  x   割られる数
 *  @param  y   割る数
 *  @return 余り
*/
double user_fmod(double x, double y)
{
    int numberX = x * 100000;
    int numberY = y * 100000;

    return  (numberX % numberY) / (float)100000;
}

/**
 *  @brief  範囲内に収まっていない場合に、値を範囲内に収める
 *  @param  value   対象の値
 *  @param  minValue    最小値
 *  @param  maxValue    最大値
 *  @return 余り
*/
float adjustValue(float value, float minValue, float maxValue)
{
    if (minValue > maxValue) {
        float   valueTemp = maxValue;
        maxValue = minValue;
        minValue = valueTemp;
    }

    if ((minValue <= value) && (value <= maxValue)) {
        //  調整の必要が無い
        return  value;
    }

    if (minValue == -1 * maxValue) {
        //  最大値と最小値の絶対値が同じ場合
        float   valueModified = user_fmod(value, maxValue);
        if (value > maxValue) {
            value = minValue + valueModified;
        } else {
            value = maxValue + valueModified;
        }

        return  value;
    }

    if (minValue > value) {
        return  minValue;
    }

    if (value > maxValue) {
        return  maxValue;
    }

    return  value;
}

/**
 * 弧度法表記を度数法表記に変換する
 * @param[in]   radian  角度[ラジアン]
 * @return  角度[度]
 */
float radian2degree(float radian)
{
    return  radian * 180.0 / Pi;
}

/**
 * 度数法表記を弧度法表記に変換する
 * @param[in]   degree  角度[度]
 * @return  角度[ラジアン]
 */
float degree2radian(float degree)
{
    return  degree * Pi / 180.0 ;
}

/**
 * 最小値を返す
 * @param[in]   value1  値1
 * @param[in]   value2  値2
 * @param[in]   value3  値3
 * @return  最小値
 */
double getMinimumValue(double value1, double value2, double value3)
{
    double minimumValue = (value1 < value2) ? value1 : value2;
    minimumValue = (minimumValue < value3) ? minimumValue : value3;

    return  minimumValue;
}

/**
 * 最大値を返す
 * @param[in]   value1  値1
 * @param[in]   value2  値2
 * @param[in]   value3  値3
 * @return  最大値
 */
double getMaximumValue(double value1, double value2, double value3)
{
    double maximumValue = (value1 > value2) ? value1 : value2;
    maximumValue = (maximumValue > value3) ? maximumValue : value3;

    return  maximumValue;
}
