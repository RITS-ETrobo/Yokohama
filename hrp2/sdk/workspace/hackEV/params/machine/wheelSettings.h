/**
 * @file    wheelSettings.h
 * @brief   This file has the settings related to wheel of EV3.
 */
#pragma once

//! ターゲット依存の定義
#include "ev3api.h"
#include "target_test.h"
#include "user_function.h"

//! \addtogroup 距離計算要素
//@{
//! タイヤの直径[単位 : cm]
extern const float EV3_WHEEL_DIAMETER;

//! 左右のタイヤ間のトレッド[単位 : cm]
extern const float EV3_TREAD;

extern float Diameter_corerctFactor;

extern float Tread_correctFactor;

extern float EV3_WHEEL_DIAMETER_inCorrectFact;

extern float EV3_TREAD_inCorrectFact;
//@}
