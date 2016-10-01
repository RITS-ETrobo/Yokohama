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

//! Lコース（スタート～懸賞入口）
extern const scenario_coordinate test_coordinate[15];
