/**
 * @file    defines.h
 * @brief   This file has the settings for each port related to EV3.
 */
#pragma once

//! ターゲット依存の定義
#include "target_test.h"

//! \addtogroup ポートの設定
//@{
//! Touch sensor
extern const sensor_port_t touch_sensor;

//! Color sensor
extern const sensor_port_t color_sensor;

//! Gyro sensor
extern const sensor_port_t gyro_sensor;

//! Sonar sensor
extern const sensor_port_t sonar_sensor;

//! Left motor
extern const motor_port_t left_motor;

//! Right motor
extern const motor_port_t right_motor;

//! Arm motor
extern const motor_port_t arm_motor;
//@}
