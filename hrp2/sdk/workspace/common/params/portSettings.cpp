/**
 * @file    portSettings.cpp
 * @brief   This file has the settings for each port related to EV3.
 */

#include "ev3api.h"
#include "portSettings.h"

//! \addtogroup ポートの設定
//@{
//! Touch sensor    :   Port 1
const sensor_port_t EV3_SENSOR_TOUCH = EV3_PORT_1;

//! Color sensor    :   Port 2
const sensor_port_t EV3_SENSOR_COLOR = EV3_PORT_2;

//! Sonar sensor    :   Port 3
const sensor_port_t EV3_SENSOR_SONAR = EV3_PORT_3;

//! Gyro sensor     :   Port 4
const sensor_port_t EV3_SENSOR_GYRO = EV3_PORT_4;

//! Left motor  :   Port C
const motor_port_t EV3_MOTOR_LEFT = EV3_PORT_C;

//! Right motor :   Port B
const motor_port_t EV3_MOTOR_RIGHT = EV3_PORT_B;

//! Arm motor :   Port A
const motor_port_t EV3_MOTOR_ARM = EV3_PORT_A;
//@}
