/**
 * @file    ColorSensorController.h
 * @brief   カラーセンサーを制御する
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! ターゲット依存の定義
#include "target_test.h"

//  関数のプロトタイプ宣言
extern void viewColor();
extern colorid_t getColorName();
extern rgb_raw_t getColorRGBraw();
#ifdef __cplusplus
}
#endif
