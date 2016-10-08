/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#pragma once

#include "runPattern.h"
#include "stopCondition.h"
#include "product.h"

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
    float direction;

    //! 走行パターン
    enum runPattern pattern;

    //! 走行シナリオが完了した時に急停止するか(trueの場合)
    bool stop;

    //! 曲率半径
    float curvatureRadius;

    //! 停止条件とするもの
    enum stopCondition stopConditionPattern;

    //! 停止条件カラー(COLOR_NONEが設定されている場合は停止判定しない)
    uint8_t stopColorID;
} scenario_running;

//! Lコース（スタート～懸賞入口）
extern const scenario_running L_Start_Sweepstakes_scenario[2];

#if FALSE
//! Lコース（懸賞入口～星取り入口）
extern const scenario_running L_Sweepstakes_starUP_scenario[];

//! Lコース（星取り）
extern const scenario_running L_StarUP_scenario[];

//! Lコース（星取り入口～ET相撲）
extern const scenario_running L_StarUP_Sumo_scenario[];

//! Lコース（ET相撲. 4つ落とすシナリオ.）
extern const scenario_running L_Sumo_scenario[23];
#endif  //  FALSE

//! Lコース（ET相撲. 赤以外を落とすシナリオ. ）
extern const scenario_running L_Sumo_scenario_hoshi_red[19];

//! Lコース（ET相撲. 青以外を落とすシナリオ. ）
extern const scenario_running L_Sumo_scenario_hoshi_blue[19];

//! Lコース（ET相撲. 緑以外を落とすシナリオ. ）
extern const scenario_running L_Sumo_scenario_hoshi_green[19];

//! Lコース（ET相撲. 黄以外を落とすシナリオ. ）
extern const scenario_running L_Sumo_scenario_hoshi_yellow[19];

#if FALSE
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

//! 検証用シナリオ(その場360度回転・左回転)
extern const scenario_running run_scenario_test_pinWheel_left[1];

//! 検証用シナリオ(その場360度回転・右回転)
extern const scenario_running run_scenario_test_pinWheel_right[1];

//! 検証用シナリオ(1m直進トレース・誤差検証用)
extern const scenario_running run_scenario_test_straght[1];

//! 検証用シナリオ(前方へ直進)
extern const scenario_running run_scenario_test_straght_NoTrace[19];

//! 検証用シナリオ(後方に直進)
extern const scenario_running run_scenario_test_straght_NoTrace_Back[19];
#endif  //  FALSE

//! 検証用シナリオ(5m直進・速度検証用)
extern const scenario_running run_scenario_test_correct_straght_NoTrace[5];

#if FALSE
//! 検証用シナリオ(30m直進・ログ監視タスク評価用)
extern const scenario_running run_scenario_straight_30m[1];
#endif  //  FALSE

//! 検証用シナリオ(ゼロヨン専用)
extern const scenario_running run_scenario_zeroyon[2];

#if FALSE
//! 検証用シナリオ(曲線走行90°になったら停止)
extern const scenario_running run_scenario_curve_90[5];

//! 検証用シナリオ(Lコースをトレースせずに走る)
extern const scenario_running run_scenario_LCource_Try[8];
#endif  //  FALSE

//! 検証用(ラインを使って向きを整える)
extern const scenario_running correct_direction_line[3];
//! 自己位置推定Positionの検証用
extern const scenario_running run_scenario_test_position[5];

extern const scenario_running back[1];