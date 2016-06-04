/**
 * @file    utilities.h
 * @brief   This file has the declarations of utilitiy functions and defines.
 */

#pragma once

//! ターゲット依存の定義
#include "target_test.h"

//! \addtogroup ポートの設定
//@{
//! Touch sensor
extern const int touch_sensor;

//! Color sensor
extern const int color_sensor;

//! Left motor
extern const int left_motor;

//! Right motor
extern const int right_motor;
//@}

//  関数のプロトタイプ宣言
#ifndef TOPPERS_MACRO_ONLY
    extern void configure_motors();
    extern void configure_sensors();
    extern int calibrate_light_intensity();
#endif  //  TOPPERS_MACRO_ONLY
