/**
 * @file    ArmModule.h
 * @brief   This file has ArmModule
 */
#pragma once

//! ターゲット依存の定義
#include "target_test.h"

//  関数のプロトタイプ宣言
extern void	initialize_arm();
extern void	move_arm(int power, int direction);
extern void	stop_arm();

