/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#include "parameterPID.h"
#include "coordinateScenario.h"

//! 検証用
const scenario_coordinate test_coordinate[14] = {
    {30, 0, 253.7143},
    {30, -41.71428571, 253.1428571},
    {30, -41.71428571, 120.5714286},
    {30, -80, 80},
    {30, -168.5714286, 80},
    {30, -225.1428571, 120.5714286},
    {30, -225.1428571, 268.5714286},
    {30, -132.5714286, 259.4285714},
    {30, -132.5714286, 116},
    {30, -84, 80},
    {30, -41.14285714, 119.4285714},
    {30, -41.14285714, 283.4285714},
    {30, 0, 252.5714286},
    {30, 0, -29.14285714}
};

////////////////////////////////////////// Lコース用座標
//! スタートしてから相撲の新幹線前まで
const scenario_coordinate start_beforeSumo[9] = {
    {30, 0, 253.7143},
    {30, -41.71428571, 253.1428571},
    {30, -41.71428571, 120.5714286},
    {30, -80, 80},
    {30, -168.5714286, 80},
    {30, -225.1428571, 120.5714286},
    {30, -225.1428571, 268.5714286},
    {30, -132.5714286, 259.4285714},
    {30, -132.5714286, 222.2857143}
};

//! 相撲台の上まで移動
const scenario_coordinate onStageSumo[1]={
    {30, -132.5714286, 182.5714286}
};

//! 相撲が終わってから懸賞まで
const scenario_coordinate fromSumo_toKensho[4]={
    {30, -132.5714286, 116},
    {30, -84, 80},
    {30, -41.14285714, 119.4285714},
    {30, -41.14285714, 283.4285714}
};

//! 懸賞からゴール
const scenario_coordinate fromKensho_toGoal[2]={
    {30, 0, 252.5714286},
    {30, 0, -29.14285714}
};
//////////////////////////////////////ここまでLコース