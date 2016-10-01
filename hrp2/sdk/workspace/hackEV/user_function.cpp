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
