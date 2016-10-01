/**
 * @file    user_function.cpp
 * @brief   imgビルドでエラーになる関数を自作した
 */
#include "user_function.h"

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
