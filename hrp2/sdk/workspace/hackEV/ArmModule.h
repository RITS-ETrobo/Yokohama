/**
 * @file    ArmModule.h
 * @brief   This file has ArmModule
 */
#pragma once

//! ターゲット依存の定義
#include "target_test.h"

//  関数のプロトタイプ宣言
extern void initialize_arm();
extern ER move_arm(int degrees, uint32_t speed_abs, bool_t blocking);
extern ER stop_arm();
extern ER initialize_arm_position();
