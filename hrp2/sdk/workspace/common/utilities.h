/**
 * @file    utilities.h
 * @brief   This file has the declarations of utilitiy functions and defines.
 */
#pragma once

#include <math.h>
//! ターゲット依存の定義
#include "target_test.h"

#include "portSettings.h"
#include "wheelSettings.h"
#include "runPattern.h"
#include "parameterPID.h"

#include "MotorWheel.h"
#include "Logger.h"
#include "LCDController.h"

//! DEBUG実行しない場合は、コメントにする事
#define DEBUG

#ifdef DEBUG
    #define _debug(x) (x)
#else
    #define _debug(x)
#endif  //  DEBUG

//! \addtogroup 白黒判定で用いる変数
//@{
//! 白の光量値
extern int white;

//! 黒の光量値
extern int black;
//@}

//  関数のプロトタイプ宣言
extern void configure_motors();
extern void configure_sensors();
extern int calibrate_light_intensity();
extern void writeFloatLCD(float value);
extern bool isGreaterAbsoluteValue(float targetValue, float compareValue);
extern void confirmBattery(bool isOutputLog = false);
extern void calibrateBW();
extern void viewColor();
