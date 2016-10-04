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
uint8_t getMinimumValue(uint8_t value1, uint8_t value2, uint8_t value3);
uint8_t getMaximumValue(uint8_t value1, uint8_t value2, uint8_t value3);

#ifdef __cplusplus
}
#endif
