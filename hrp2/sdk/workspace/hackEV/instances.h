/**
 * @file    instances.h
 * @brief   This file has the declarations of instances.
 */
#pragma once

#include "Logger.h"
#include "MotorWheel.h"

//! ログクラスのインスタンス
extern Logger*  logger;

//! 左ホイールクラスのインスタンス
extern MotorWheel *motorWheelLeft;

//! 右ホイールクラスのインスタンス
extern MotorWheel *motorWheelRight;
