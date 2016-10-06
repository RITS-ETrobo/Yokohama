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

//! タイヤ径の補正値(EV3_WHEEL_DIAMETERに掛け算)
float Diameter_corerctFactor = 1.0F;

//! トレッドの補正値(TREADは角度計算では分母にくるため、EV3_TREADに割り算!)
float Tread_correctFactor = 1.0F;
//@}
