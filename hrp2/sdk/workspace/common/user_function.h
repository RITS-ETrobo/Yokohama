/**
 * @file    SpeedCalculator.h
 * @brief   平均速度を求める
 */
#pragma once

#include "product.h"

//! 円周率
extern const float Pi;

#ifdef __cplusplus
extern "C" {
#endif

double user_fmod(double x, double y);
float adjustValue(float value, float minValue, float maxValue);
float radian2degree(float radian);
float degree2radian(float degree);
double getMinimumValue(double value1, double value2, double value3);
double getMaximumValue(double value1, double value2, double value3);

#ifdef __cplusplus
}
#endif
