/**
 * @file    instances.h
 * @brief   This file has the declarations of instances.
 */
#pragma once

#include "product.h"
#include "Logger.h"
#include "DriveController.h"
#include "GyroSensorController.h"
#include "SonarSensorController.h"
#include "ScenarioManajer.h"

//! ログクラスのインスタンス
extern Logger*  logger;

//! DriveControllerクラスのインスタンス
extern DriveController* driveController;

#ifndef EV3_UNITTEST
    //! Clockクラスのインスタンス
    extern Clock    *clock;
#endif  // EV3_UNITTEST

//! GyroSensorControllerクラスのインスタンス
extern GyroSensorController* gyroSensorController;

//! SonarSensorControllerクラスのインスタンス
extern SonarSensorController    *sonarSensorController;

//! ScenarioManajerクラスのインスタンス　※エラーになってしまう
// extern ScenarioManajer    *scenarioManajer;