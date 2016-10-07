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
//! スタートしてから星取り前の直線まで
const scenario_coordinate start_straightBeforeHoshitori[6] = {
    {50, -72, 1163},
    {50, -213, 1163},
    {50, -213, 566},
    {50, -778, 566},
    {50, -857, 659},
    {50, -857, 920}
};

//! 灰色にかかるところ（グレー検知でも止まるように）
const scenario_coordinate toGRAYBeforeHOSHI[1] ={
    {20, -857, 1190,COLOR_GRAY}
};

//! 星取り
const scenario_coordinate toHoshitori [1] ={
    {20, -857, 1226}
};

//! 相撲前の線路前まで
const scenario_coordinate beforeSUMO[2] = {
    {50, -535, 1162},
    {50, -535, 1025}
};


//! 相撲台の上まで移動
const scenario_coordinate onStageSumo[1]={
    {50, -535, 891}
};

//! 相撲が終わってから懸賞前の直線まで
const scenario_coordinate fromSumo_toKenshoStraight[3]={
    {50, -535, 566},
    {50, -214, 566},
    {50, -214, 997}
};

//! 懸賞前の灰色(カラーでも止まる)
const scenario_coordinate toGRAYBeforeKENSHO[1] ={
    {50, -214, 1217}
};

//! 懸賞
const scenario_coordinate KENSHO[1]={
    {50, -214, 1259}
};

//! 懸賞からゴール
const scenario_coordinate fromKensho_toGoal[2]={
    {50, -72, 1150},
    {50, -72, 76}
};
//////////////////////////////////////ここまでLコース