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

//  関数のプロトタイプ宣言
#ifndef TOPPERS_MACRO_ONLY
    extern void initialize_pid_controller();
    extern void pid_controller_task(intptr_t exinf);
#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
