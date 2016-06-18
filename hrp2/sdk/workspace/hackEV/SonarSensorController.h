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

//! \addtogroup ポートの設定
//@{
//! Sonar sensor
extern const sensor_port_t sonar_sensor;

//@}

//  関数のプロトタイプ宣言
#ifndef TOPPERS_MACRO_ONLY
    extern void	initialize_sonarsensor();
    extern void control_sonarsensor();

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
