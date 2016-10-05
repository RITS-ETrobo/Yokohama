/**
 * @file    gyroboy.h
 * @brief   This file has the gyro controll feature.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! ターゲット依存の定義
#include "target_test.h"

//  関数のプロトタイプ宣言
extern void balance_task(intptr_t exinf);
extern void idle_task(intptr_t exinf);
extern void drive_controller_task(intptr_t exinf);

#ifdef __cplusplus
}
#endif
