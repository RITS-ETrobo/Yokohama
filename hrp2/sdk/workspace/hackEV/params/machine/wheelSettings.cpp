/**
 * @file    wheelSettings.cpp
 * @brief   This file has the settings related to wheel of EV3.
 */

#include "wheelSettings.h"

//! \addtogroup 距離計算要素
//@{
//! タイヤの直径[単位 : cm]
const float EV3_WHEEL_DIAMETER = 8.2F;

//! 左右のタイヤ間のトレッド[単位 : cm]
const float EV3_TREAD = 13.25F;

//! タイヤ径の補正値
float Diameter_corerctFactor = 1.0F;

//! トレッドの補正値
float Tread_correctFactor = 1.0F;

//! タイヤの直径[単位 : cm]（※補正値含む）
float EV3_WHEEL_DIAMETER_inCorrectFact = EV3_WHEEL_DIAMETER * Diameter_corerctFactor;

//! 左右のタイヤ間のトレッド[単位 : cm]（※補正値含む）(TREADは角度計算では分母にくるため、割り算!)
float EV3_TREAD_inCorrectFact = EV3_TREAD / Tread_correctFactor;

//@}
