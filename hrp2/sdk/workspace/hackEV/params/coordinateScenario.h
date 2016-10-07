/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#pragma once

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
} scenario_coordinate;

//! 検証用
extern const scenario_coordinate test_coordinate[14];

//! スタートしてから相撲まで
extern const scenario_coordinate start_straightBeforeHoshitori[7];

//! 星取りまで
extern const scenario_coordinate toHoshitori [1];

//! 相撲台の上まで移動
extern const scenario_coordinate onStageSumo[1];

//! 相撲が終わってから懸賞まで
extern const scenario_coordinate fromSumo_toKensho[4];

//! 懸賞からゴール
extern const scenario_coordinate fromKensho_toGoal[2];