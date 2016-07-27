/**
 * @file    lcdSettings.cpp
 * @brief   This file has the settings related to LCD of EV3.
 *          LCDに関するAPIの詳細は、EV3RT C API Referenceを参照する事
 */
#pragma once

//! ターゲット依存の定義
#include "ev3api.h"
#include "target_test.h"

//! \addtogroup LCDに関連した定義
//@{
//! 1行辺りの高さ : フォントMEDIUM
extern const unsigned char fontHeightMedium;

//! 1行辺りの高さ : フォントSMALL
extern const unsigned char fontHeightSmall;
//@}
