/**
 * @file    instances.h
 * @brief   This file has the declarations of instances.
 */
#pragma once

#include "product.h"
#include "Logger.h"
#include "DriveController.h"

//! ログクラスのインスタンス
extern Logger*  logger;

//! DriveControllerクラスのインスタンス
extern DriveController* driveController;

#ifndef EV3_UNITTEST
    //! Clockクラスのインスタンス
    extern Clock    *clock;
#endif  // EV3_UNITTEST
