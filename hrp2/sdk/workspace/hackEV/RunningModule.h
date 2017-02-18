/**
 * @file    RunningModule.h
 * @brief   This file has RunningModule
 */
#pragma once

//! ターゲット依存の定義
#include "target_test.h"
#include "coordinateScenario.h"

//  関数のプロトタイプ宣言
extern void	initialize_run();
extern void start_run();
extern void start_run_test();
extern void start_LcourseRun();
extern void start_RcourseRun();
extern void start_RcourseRunShortCut();
extern void BuleRootTry();
extern int readCoordinateFile(scenario_coordinate coordinateDatas[40]);