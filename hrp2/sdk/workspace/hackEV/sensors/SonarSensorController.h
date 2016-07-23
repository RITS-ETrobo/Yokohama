/**
 * @file    SonarSensorController.h
 * @brief   This file has SonarSensorController
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! ターゲット依存の定義
#include "target_test.h"

//  関数のプロトタイプ宣言
extern void	initialize_sonarsensor();
extern void control_sonarsensor();

extern void setEnabledSonarSensor(bool _enabledSonarSensor);

#ifdef __cplusplus
}
#endif
