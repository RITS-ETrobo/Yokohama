/**
 * @file    pid_controller.h
 * @brief   This file has the declarations of PID contoller functions and defines.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! ターゲット依存の定義
#include "target_test.h"

//! 白の光量値
extern int white;

//! 黒の光量値
extern int black;

//! 白と黒の光量値の平均値
extern float midpoint;

typedef struct{
    float kP;
    float kI;
    float kD;
} PID_PARAMETER;

//  関数のプロトタイプ宣言
extern void initialize_pid_controller();
extern void pid_controller_task(intptr_t exinf);
extern float pid_controller(PID_PARAMETER pidParameter);
#ifdef __cplusplus
}
#endif
