/**
 * @file    instances.h
 * @brief   This file has the declarations of instances.
 */
#pragma once

#include "Logger.h"
#include "MotorWheel.h"
#include "Clock.h"

//! ログクラスのインスタンス
extern Logger*  logger;

//! Clockクラスのインスタンス
extern Clock    *clock;

//! 左ホイールクラスのインスタンス
extern MotorWheel   *motorWheelLeft;

//! 右ホイールクラスのインスタンス
extern MotorWheel   *motorWheelRight;
