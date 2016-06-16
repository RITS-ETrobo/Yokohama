/**
 * @file    UltrasonicController.h
 * @brief   This file has UltrasonicController
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! ターゲット依存の定義
#include "target_test.h"


//  関数のプロトタイプ宣言
#ifndef TOPPERS_MACRO_ONLY
    extern void	initialize_ultrasonic();
    extern void control_ultrasonic();

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
