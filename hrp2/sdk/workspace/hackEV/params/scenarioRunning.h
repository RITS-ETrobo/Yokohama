/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#pragma once

#include "runPattern.h"

/**
 * @struct scenario_running
 * 走行シナリオ
 */
typedef struct {
    //! 出力(-100～+100)
    int power;

    //! 走行距離[cm]
    float distance;

    //! 向き。使わない場合は、-1。使う場合は、0～360
    int direction;

    //! 走行パターン
    enum runPattern pattern;

    //! 走行シナリオが完了した時に急停止するか(trueの場合)
    bool stop;
} scenario_running;

//! Lコース（スタート～懸賞入口）
extern const scenario_running L_Start_Sweepstakes_scenario[2];

//! Lコース（懸賞入口～星取り入口）
extern const scenario_running L_Sweepstakes_starUP_scenario[];

//! Lコース（星取り）
extern const scenario_running L_StarUP_scenario[];

//! Lコース（星取り入口～ET相撲）
extern const scenario_running L_StarUP_Sumo_scenario[];

//! Lコース（ET相撲）※この間にゲーム
extern const scenario_running L_Sumo_scenario[];

//! Lコース（ET相撲後～懸賞運び入口）
extern const scenario_running L_Sumo_kensho_scenario[];

//! Lコース（懸賞運び）
extern const scenario_running L_kensho_scenario[];

//! Lコース（懸賞運び～ゴール）
extern const scenario_running L_kensho_Goal_scenario[];

//! Rコース(スタート～難所入口)
extern const scenario_running R_Start_enterGameArea_scenario[];

//! Rコース(難所入口～ゲームエリア入口)
extern const scenario_running L_enterGameArea_GameArea_scenario[];

//! Rコース(ゲームエリア出口～LAP)
extern const scenario_running R_exitGameArea_LAP_scenario[];

//! Rコース(LAP～GOAL)
extern const scenario_running R_LAP_GOAL_scenario[];

//! 検証用シナリオ
extern const scenario_running run_scenario_test[];

//! 検証用シナリオ（ラインの縁の変更）
extern const scenario_running run_scenario_test_switch[];

//! 検証用シナリオ(片側回転でのUターン)
extern const scenario_running run_scenario_test_UTurn[3];

//! 検証用シナリオ(右側走行)
extern const scenario_running run_scenario_test_right[];

//! 検証用シナリオ(その場360度回転・誤差検証用)
extern const scenario_running run_scenario_test_pinWheel[1];

//! 検証用シナリオ(1m直進トレース・誤差検証用)
extern const scenario_running run_scenario_test_straght[1];

//! 検証用シナリオ(1m直進・誤差検証用)
extern const scenario_running run_scenario_test_straght_NoTrace[19];

//! 検証用シナリオ(2m直進・誤差検証用)
extern const scenario_running run_scenario_test_correct_straght_NoTrace[1];

//! 検証用シナリオ(30m直進・ログ監視タスク評価用)
extern const scenario_running run_scenario_straight_30m[1];
