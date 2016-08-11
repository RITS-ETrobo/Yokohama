/**
 * @file    instances.h
 * @brief   This file has the declarations of instances.
 */
#pragma once

#include "Logger.h"
#include "DriveController.h"
#include "Clock.h"

//! ログクラスのインスタンス
extern Logger*  logger;

//! DriveControllerクラスのインスタンス
extern DriveController* driveController;

//! Clockクラスのインスタンス
extern Clock    *clock;
