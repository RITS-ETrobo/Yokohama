/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#pragma once

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"
#include "coordinateScenario.h"
#include "orientationPattern.h"
#include "EV3Position.h"


/**
 * @struct scenario_coordinate
 * 走行シナリオ
 */
typedef struct {
    //! 出力(-100～+100)
    int power;

    //! 目標点X座標
    float targetX;

    //! 目標点Y座標
    float targetY;

    //! 停止条件カラー(COLOR_NONEが設定されている場合は停止判定しない)
    uint8_t stopColorID;
} scenario_coordinate;

//! 検証用
extern const scenario_coordinate test_coordinate[14];

//! スタートしてから相撲まで
extern const scenario_coordinate start_straightBeforeHoshitori[6];

//! 星取り前の灰色（カラーでもとめる）
extern const scenario_coordinate toGRAYBeforeHOSHI[2];

//! 星取りまで
extern const scenario_coordinate toHoshitori [1];

//! 相撲前の線路前まで
extern const scenario_coordinate beforeSUMO[2];

//! 相撲台の上まで移動
extern const scenario_coordinate onStageSumo[1];

//! 相撲が終わってから懸賞までの直線
extern const scenario_coordinate fromSumo_toKenshoStraight[3];

//! 検証前のグレーゾーン
extern const scenario_coordinate toGRAYBeforeKENSHO[2];

//! 懸賞まで
extern const scenario_coordinate KENSHO[1];

//! 懸賞からゴール
extern const scenario_coordinate fromKensho_toGoal[2];