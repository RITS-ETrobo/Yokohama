/**
 * @file    portSettings.h
 * @brief   This file has the settings for each port related to EV3.
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

//! \addtogroup ポートの設定
//@{
//! Touch sensor
extern const sensor_port_t EV3_SENSOR_TOUCH;

//! Color sensor
extern const sensor_port_t EV3_SENSOR_COLOR;

//! Gyro sensor
extern const sensor_port_t EV3_SENSOR_GYRO;

//! Sonar sensor
extern const sensor_port_t EV3_SENSOR_SONAR;

//! Left motor
extern const motor_port_t EV3_MOTOR_LEFT;

//! Right motor
extern const motor_port_t EV3_MOTOR_RIGHT;

//! Arm motor
extern const motor_port_t EV3_MOTOR_ARM;
//@}
