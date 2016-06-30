/**
 * @file    RunningModule.h
 * @brief   This file has RunningModule
 */
#pragma once

//! ターゲット依存の定義
#include "target_test.h"

//  関数のプロトタイプ宣言
extern void	initialize_run();
extern void start_run(int power,int corveAngle);
extern void run_withPID();
