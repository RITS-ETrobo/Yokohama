/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#include "runPattern.h"
#include "parameterPID.h"
#include "scenarioRunning.h"

//! Lコース（スタート～懸賞入口）
const scenario_running L_Start_Sweepstakes_scenario[2] = {
    {30, 246.0F, -1, TRACE_STRAIGHT_RIGHT, false },
    {30, 64.5F, -1, TRACE_CURVE_RIGHT, true }
};
 
//! Lコース（懸賞入口～星取り入口）
const scenario_running L_Sweepstakes_starUP_scenario[] = {
    {30, 127.5F, -1, TRACE_STRAIGHT, false},
    {30, 63.5F, -1, TRACE_CURVE, false},
    {30, 25.0F, -1, TRACE_STRAIGHT, false},
    {30, 32.5F, -1, TRACE_CURVE, false},
    {30, 26.7F, -1, TRACE_CURVE, false},
    {30, 24.0F, -1, TRACE_CURVE, false},
    {30, 118.5F, -1, TRACE_STRAIGHT, false}
};
 
//! Lコース（星取り）
const scenario_running L_StarUP_scenario[] = {
    {30, 20.8F, -1, NOTRACE_STRAIGHT, false},
    {30, 15.2F, -1, TRACE_STRAIGHT, false},
    {30, 3.0F, -1, TRACE_STRAIGHT, false},
    {-30, 39.0F, -1, NOTRACE_STRAIGHT, false}
};
 
//! Lコース（星取り入口～ET相撲）
const scenario_running L_StarUP_Sumo_scenario[] = {
    {30, 16.4F, -1, TRACE_CURVE, false},
    {30, 21.2F, -1, TRACE_CURVE, false},
    {30, 6.5F, -1, TRACE_CURVE, false},
    {30, 46.0F, -1, TRACE_STRAIGHT, false},
    {30, 12.1F, -1, TRACE_CURVE, false},
    {30, 10.5F, -1, TRACE_CURVE, false}
};

//! Lコース（ET相撲）※この間にゲーム
const scenario_running L_Sumo_scenario[] = {
    {30, 134.2F, -1, TRACE_STRAIGHT, false}
};

//! Lコース（ET相撲後～懸賞運び入口）
const scenario_running L_Sumo_kensho_scenario[] = {
    {30, 26.5F, -1, TRACE_CURVE, false},
    {30, 24.9F, -1, TRACE_CURVE, false},
    {30, 17.0F, -1, TRACE_CURVE, false},
    {30, 67.0F, -1, TRACE_CURVE, false},
    {30, 127.5F, -1, TRACE_STRAIGHT, false}
};

//! Lコース（懸賞運び）
const scenario_running L_kensho_scenario[] = {
    {30, 22.0F, -1, NOTRACE_STRAIGHT, false},
    {30, 11.5F, -1, TRACE_STRAIGHT, false},
    {-30, 11.5F, -1, TRACE_STRAIGHT, false},
    {-30, 22.0F, -1, NOTRACE_STRAIGHT, true}
};

//! Lコース（懸賞運び～ゴール）
const scenario_running L_kensho_Goal_scenario[] = {
    {30, 64.5F, -1, TRACE_CURVE, false},
    {30, 250.0F, -1, TRACE_STRAIGHT, false}
};

//! Rコース(スタート～難所入口)
const scenario_running R_Start_enterGameArea_scenario[] = {
    {30, 44.8F, -1, TRACE_STRAIGHT, false},
    {30, 24.8F, -1, TRACE_CURVE, false}
};

//! Rコース(難所入口～ゲームエリア入口)
const scenario_running L_enterGameArea_GameArea_scenario[] = {
    {30, 37.6F, -1, NOTRACE_STRAIGHT, false}
};

//! Rコース(ゲームエリア出口～LAP)
const scenario_running R_exitGameArea_LAP_scenario[] = {
    {30, 30.4F, -1, TRACE_STRAIGHT, false},
    {30, 25.8F, -1, TRACE_STRAIGHT, false},
    {30, 36.6F, -1, TRACE_CURVE, false},
};

//! Rコース(LAP～GOAL)
const scenario_running R_LAP_GOAL_scenario[] = {
    {30, 81.1F, -1, TRACE_STRAIGHT, false},
    {30, 103.9F, -1, TRACE_STRAIGHT, false},
    {30, 47.7F, -1, TRACE_CURVE, false},
    {30, 48.6F, -1, TRACE_CURVE, false},
    {30, 131.5F, -1, TRACE_STRAIGHT, false},
    {30, 3.8F, -1, TRACE_CURVE, false},
    {30, 47.6F, -1, TRACE_CURVE, false},
    {30, 4.0F, -1, TRACE_CURVE, false},
    {30, 190.1F, -1, TRACE_STRAIGHT, false},
    {30, 32.8F, -1, TRACE_CURVE, false},
    {30, 297.0F, -1, TRACE_STRAIGHT, false},
};

//! 検証用シナリオ
const scenario_running run_scenario_test[] = {
    {60, 41.0F, -1, TRACE_STRAIGHT, false},
    {30, 43.0F, -1, TRACE_CURVE, false},
    {60, 40.0F, -1, TRACE_STRAIGHT, false},
    {30, 195.0F, -1, TRACE_CURVE, false},
    {60, 0.0F, 360, PINWHEEL, true}
};

//! 検証用シナリオ（ラインの縁の変更）
const scenario_running run_scenario_test_switch[] = {
    {20, 10.0F, -1, TRACE_STRAIGHT_RIGHT, false},
    {20, 0.0F, -1, SWITCH_SIDE_LEFT, false},
    {20, 15.0F, -1, SWITCH_SIDE_LEFT, true}
};

//! 検証用シナリオ(片側回転でのUターン)
const scenario_running run_scenario_test_UTurn[3] = {
    {40, 0.0F, 180, ONESIDE_PINWHEEL_RIGHT, false},
    {40, 100.0F, -1, NOTRACE_STRAIGHT, false},
    {40, 100.0F, 180, ONESIDE_PINWHEEL_LEFT, true}
};

//! 検証用シナリオ(右側走行)
const scenario_running run_scenario_test_right[] = {
    {20, 100.0F, -1, TRACE_STRAIGHT, true}
};

//! 検証用シナリオ(ライン復帰)
const scenario_running run_scenario_test_find_Line[1] = {
    {30, 0.0F, -1, FIND_LINE, true}
};