/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#include "parameterPID.h"
#include "coordinateScenario.h"

#include "ColorSensorController.h"

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
const scenario_coordinate start_straightBeforeHoshitori[2] = {
    {45, -72, 1163},
    {45, -213, 1163, COLOR_BLACK}
};

//! 最初のカーブを曲がってから星取り前まで
const scenario_coordinate fromfirstCurve[4]={
    {45, -213, 566},
    {45, -778, 566},
    {45, -857, 659},
    {45, -857, 920}
};

//! 灰色にかかるところ（グレー検知でも止まるように）
const scenario_coordinate toGRAYBeforeHOSHI[2] ={
    {15, -857, 1157},
    {15, -857, 1190, COLOR_GRAY}
};

//! 星取り
const scenario_coordinate toHoshitori [1] ={
    {15, -857, 1226}
};

//! 相撲前の線路前まで
const scenario_coordinate beforeSUMO[2] = {
    {45, -535, 1162},
    {45, -535, 1020}
};


//! 相撲台の上まで移動
const scenario_coordinate onStageSumo[1]={
    {45, -535, 885}
};

//! 相撲が終わってから懸賞前の直線まで
const scenario_coordinate fromSumo_toKenshoStraight[3]={
    {45, -535, 566},
    {45, -214, 566},
    {45, -214, 815}
};

//! 懸賞前の灰色(カラーでも止まる)
const scenario_coordinate toGRAYBeforeKENSHO[2] ={
    {25, -214,  1203},
    {25, -214, 1217, COLOR_GRAY}
};

//! 懸賞
const scenario_coordinate KENSHO[1]={
    {45, -214, 1259}
};

//! 懸賞からゴール
const scenario_coordinate fromKensho_toGoal[2]={
    {45, -72, 1150},
    {45, -72, 76}
};
//////////////////////////////////////ここまでLコース

////////////////////////////////////////////ここからRコース
//! スタートから格子手前まで
const scenario_coordinate toBeforeKOUSHI[1] ={
    {45, 71, 538}
};

//! 青を運ぶ
const scenario_coordinate BlueRoot[2]={
    {45, 261, 466},
    {45, 573, 466}
};

//! 青の横
const scenario_coordinate nigeru[2]={
    {45, 573, 386}
};

//! 格子でてからからショートカットをするルート
const scenario_coordinate YesShortcutRoot[7] ={
    {45, 858, 552},
    {45, 858, 837},
    {45, 255, 867},
    {45, 214, 943},
    {45, 72, 959},
    {45, 72, 1189},
    {45, 72, 80}
};

//! 格子でてからショートカットしないルート
const scenario_coordinate NoShortcutRoot[8] ={
    {45, 858, 552},
    {45, 858, 837},
    {45, 255, 867},
    {45, 255, 1043},
    {45, 831, 1043},
    {45, 831, 1210},
    {45, 72, 1210},
    {45, 72, 95}
};