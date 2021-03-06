﻿/**
 * @file    scenarioRunning.cpp
 * @brief   This file has scenarios for running.
 */
#include "runPattern.h"
#include "parameterPID.h"
#include "scenarioRunning.h"
#include "stopCondition.h"

//! Lコース（スタート～懸賞入口）
const scenario_running L_Start_Sweepstakes_scenario[2] = {
    {30, 246.0F, -1, TRACE_STRAIGHT_RIGHT, false,0 ,DISTANCE_STOP},
    {30, 64.5F, -1, TRACE_CURVE_RIGHT, true,0 ,DISTANCE_STOP}
};

#if FALSE
//! Lコース（懸賞入口～星取り入口）
const scenario_running L_Sweepstakes_starUP_scenario[] = {
    {30, 127.5F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 63.5F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 25.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 32.5F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 26.7F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 24.0F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 118.5F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP}
};
 
//! Lコース（星取り）
const scenario_running L_StarUP_scenario[] = {
    {30, 20.8F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 15.2F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 3.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-30, 39.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP}
};
 
//! Lコース（星取り入口～ET相撲）
const scenario_running L_StarUP_Sumo_scenario[] = {
    {30, 16.4F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 21.2F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 6.5F, -1, TRACE_CURVE, false,0},
    {30, 46.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 12.1F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 10.5F, -1, TRACE_CURVE, false,0,DISTANCE_STOP}
};


//! Lコース（ET相撲. 4つ落とすシナリオ.）
const scenario_running L_Sumo_scenario[23] = {
    //{ 30, 50.000F,   -1, NOTRACE_STRAIGHT, false, 0}, //土俵中心まで前進する
    { 20,  0.000F,  140, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方を向く
    { 20, 11.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄る
    { 20,  0.000F,  -45, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方へ向きなおす
    { 20,  9.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //さらに近寄り、サークルを検知した(本当はカラーセンサーで前進終了判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //余裕を持って後退
    { 20,  0.000F,  180, PINWHEEL        , false, 0, DIRECTION_STOP}, //青へ向く
    { 20, 28.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //計算上は24cmだが、調整して28cm
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 17.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F,   90, PINWHEEL        , false, 0, DIRECTION_STOP}, //奥の方へ向く
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、黒ラインを検知した(本当はカラーセンサーで判定)
    { 20,  0.000F,  -90, PINWHEEL        , false, 0, DIRECTION_STOP}, //緑の方へ向く
    { 20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F,  190, PINWHEEL        , false, 0, DIRECTION_STOP}, //黄の方へ向く //本当は180
    { 20, 28.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //計算上は24cmだが、調整して28cm
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F, -200, PINWHEEL        , false, 0, DIRECTION_STOP}, //内側に向く
    { 20, 11.174F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //中間まで進む
    { 20,  0.000F,  110, PINWHEEL        , true , 0, DIRECTION_STOP}  //レールの方を向く (ここで新幹線待ち)
    //{ 20, 40.000F,   -1, NOTRACE_STRAIGHT, false, 0}  //土俵から降りる
};
#endif  //  FALSE

//! Lコース（ET相撲. 赤以外を落とすシナリオ. ）
const scenario_running L_Sumo_scenario_hoshi_red[19] = {
    //{30, 50.0F, -1, NOTRACE_STRAIGHT, false, 0},  //土俵中心まで前進する
    { 20,  0.000F, -140, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方を向く
    { 20, 11.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄る
    { 20,  0.000F,   45, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方へ向きなおす
    { 20,  9.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //さらに近寄り、サークルを検知した(本当はカラーセンサーで前進終了判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 20.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退 //17から20へ調整
    { 20,  0.000F,   90, PINWHEEL        , false, 0, DIRECTION_STOP}, //奥の方へ向く
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、黒ラインを検知した(本当はカラーセンサーで判定)
    { 20,  0.000F,  -90, PINWHEEL        , false, 0, DIRECTION_STOP}, //緑の方へ向く
    { 20, 13.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //10から13へ調整
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F,  190, PINWHEEL        , false, 0, DIRECTION_STOP}, //黄の方へ向く //本当は180でよいが調整して190
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //計算上は24cmだが、調整して30cm
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 13.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退 //計算上は10cmだが、(//17から20へ調整)に合わせて13cmに調整
    { 20,  0.000F, -200, PINWHEEL        , false, 0, DIRECTION_STOP}, //内側に向く
    { 20, 11.174F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //中間まで進む
    { 20,  0.000F,  100, PINWHEEL        , true , 0, DIRECTION_STOP}  //レールの方を向く (ここで新幹線待ち) //(//本当は180でよいが調整して190)に合わせて100に調整
    //{ 20, 40.0F,   -1, NOTRACE_STRAIGHT, false, 0}  //土俵から降りる
};

//! Lコース（ET相撲. 青以外を落とすシナリオ. ）
const scenario_running L_Sumo_scenario_hoshi_blue[19] = {
    //{30, 50.0F, -1, NOTRACE_STRAIGHT, false, 0},  //土俵中心まで前進する
    { 20,  0.000F,  140, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方を向く
    { 20, 11.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄る
    { 20,  0.000F,  -45, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方へ向きなおす
    { 20,  9.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //さらに近寄り、サークルを検知した(本当はカラーセンサーで前進終了判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 20.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退 //17から20へ調整
    { 20,  0.000F,  -90, PINWHEEL        , false, 0, DIRECTION_STOP}, //奥の方へ向く
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、黒ラインを検知した(本当はカラーセンサーで判定)
    { 20,  0.000F,   90, PINWHEEL        , false, 0, DIRECTION_STOP}, //黄の方へ向く
    { 20, 13.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //10から13へ調整
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F, -190, PINWHEEL        , false, 0, DIRECTION_STOP}, //緑の方へ向く //本当は-180でよいが調整して-190
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //計算上は24cmだが、調整して30cm
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 13.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退 //計算上は10cmだが、(//17から20へ調整)に合わせて13cmに調整
    { 20,  0.000F,  200, PINWHEEL        , false, 0, DIRECTION_STOP}, //内側に向く
    { 20, 11.174F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //中間まで進む
    { 20,  0.000F, -100, PINWHEEL        , true , 0, DIRECTION_STOP}  //レールの方を向く (ここで新幹線待ち) //(//本当は-180でよいが調整して-190)に合わせて210に調整
    //{ 20, 40.0F,   -1, NOTRACE_STRAIGHT, false, 0}  //土俵から降りる
};

//! Lコース（ET相撲. 緑以外を落とすシナリオ. ）
const scenario_running L_Sumo_scenario_hoshi_green[19] = {
    //{30, 50.0F, -1, NOTRACE_STRAIGHT, false, 0},  //土俵中心まで前進する
    { 20,  0.000F, -140, PINWHEEL        , false, 0, DIRECTION_STOP}, //青の方を向く
    { 20, 11.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄る
    { 20,  0.000F,   45, PINWHEEL        , false, 0, DIRECTION_STOP}, //青の方へ向きなおす
    { 20,  9.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //さらに近寄り、サークルを検知した(本当はカラーセンサーで前進終了判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //余裕を持って後退
    { 20,  0.000F, -180, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤へ向く
    { 20, 28.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //計算上は24cmだが、調整して28cm
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 17.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F,  -90, PINWHEEL        , false, 0, DIRECTION_STOP}, //奥の方へ向く
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、黒ラインを検知した(本当はカラーセンサーで判定)
    { 20,  0.000F,   90, PINWHEEL        , false, 0, DIRECTION_STOP}, //黄の方へ向く
    { 20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F, -200, PINWHEEL        , false, 0, DIRECTION_STOP}, //内側に向く
    { 20, 11.174F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //中間まで進む
    { 20,  0.000F,  110, PINWHEEL        , true , 0, DIRECTION_STOP}  //レールの方を向く (ここで新幹線待ち)
    //{ 20, 40.0F,   -1, NOTRACE_STRAIGHT, false, 0}  //土俵から降りる
};

//! Lコース（ET相撲. 黄以外を落とすシナリオ. ）
const scenario_running L_Sumo_scenario_hoshi_yellow[19] = {
    //{30, 50.0F, -1, NOTRACE_STRAIGHT, false, 0},  //土俵中心まで前進する
    { 20,  0.000F,  140, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方を向く
    { 20, 11.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄る
    { 20,  0.000F,  -45, PINWHEEL        , false, 0, DIRECTION_STOP}, //赤の方へ向きなおす
    { 20,  9.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //さらに近寄り、サークルを検知した(本当はカラーセンサーで前進終了判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //余裕を持って後退
    { 20,  0.000F,  180, PINWHEEL        , false, 0, DIRECTION_STOP}, //青へ向く
    { 20, 28.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定) //計算上は24cmだが、調整して28cm
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 17.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F,   90, PINWHEEL        , false, 0, DIRECTION_STOP}, //奥の方へ向く
    { 20, 30.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、黒ラインを検知した(本当はカラーセンサーで判定)
    { 20,  0.000F,  -90, PINWHEEL        , false, 0, DIRECTION_STOP}, //緑の方へ向く
    { 20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //近寄り、サークルを検知した(本当はカラーセンサーで判定)
    { 20,  7.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //前進(このあとアームで投げ飛ばし)
    {-20, 10.000F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //後退
    { 20,  0.000F,  200, PINWHEEL        , false, 0, DIRECTION_STOP}, //内側に向く
    { 20, 11.174F,   -1, NOTRACE_STRAIGHT, false, 0, DISTANCE_STOP}, //中間まで進む
    { 20,  0.000F, -110, PINWHEEL        , true , 0, DIRECTION_STOP}  //レールの方を向く (ここで新幹線待ち)
    //{ 20, 40.0F,   -1, NOTRACE_STRAIGHT, false, 0}  //土俵から降りる
};

#if FALSE
//! Lコース（ET相撲後～懸賞運び入口）
const scenario_running L_Sumo_kensho_scenario[] = {
    {30, 26.5F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 24.9F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 17.0F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 67.0F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 127.5F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP}
};

//! Lコース（懸賞運び）
const scenario_running L_kensho_scenario[] = {
    {30, 22.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 11.5F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-30, 11.5F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-30, 22.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP}
};

//! Lコース（懸賞運び～ゴール）
const scenario_running L_kensho_Goal_scenario[] = {
    {30, 64.5F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 250.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP}
};

//! Rコース(スタート～難所入口)
const scenario_running R_Start_enterGameArea_scenario[] = {
    {30, 44.8F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 24.8F, -1, TRACE_CURVE, false,0,DISTANCE_STOP}
};

//! Rコース(難所入口～ゲームエリア入口)
const scenario_running L_enterGameArea_GameArea_scenario[] = {
    {30, 37.6F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP}
};

//! Rコース(ゲームエリア出口～LAP)
const scenario_running R_exitGameArea_LAP_scenario[] = {
    {30, 30.4F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 25.8F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 36.6F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
};

//! Rコース(LAP～GOAL)
const scenario_running R_LAP_GOAL_scenario[] = {
    {30, 81.1F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 103.9F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 47.7F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 48.6F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 131.5F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 3.8F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 47.6F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 4.0F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 190.1F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 32.8F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {30, 297.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
};

//! 検証用シナリオ
const scenario_running run_scenario_test[] = {
    {60, 41.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 43.0F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {60, 40.0F, -1, TRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 195.0F, -1, TRACE_CURVE, false,0,DISTANCE_STOP},
    {60, 0.0F, 360, PINWHEEL, true,0,DISTANCE_STOP}
};

//! 検証用シナリオ（ラインの縁の変更）
const scenario_running run_scenario_test_switch[] = {
    {20, 10.0F, -1, TRACE_STRAIGHT_RIGHT, false,0,DISTANCE_STOP},
    {20, 0.0F, -1, SWITCH_SIDE_LEFT, false,0,DISTANCE_STOP},
    {20, 15.0F, -1, SWITCH_SIDE_LEFT, true,0,DISTANCE_STOP}
};

//! 検証用シナリオ(片側回転でのUターン)
const scenario_running run_scenario_test_UTurn[3] = {
    {40, 0.0F, 180, ONESIDE_PINWHEEL_RIGHT, false,0,DISTANCE_STOP},
    {40, 100.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {40, 100.0F, 180, ONESIDE_PINWHEEL_LEFT, true,0,DISTANCE_STOP}
};

//! 検証用シナリオ(右側走行)
const scenario_running run_scenario_test_right[] = {
    {20, 100.0F, -1, TRACE_STRAIGHT, true,0,DISTANCE_STOP}
};

//! 検証用シナリオ(その場360度回転・左回転)
const scenario_running run_scenario_test_pinWheel_left[1] = {
    {30, 0.0F, 360, PINWHEEL, true,0,DISTANCE_STOP}
};



//! 検証用シナリオ(1m直進トレース・誤差検証用)
const scenario_running run_scenario_test_straght[1] = {
    {30, 100.0F, -1, TRACE_STRAIGHT, true,0,DISTANCE_STOP}
};
#endif  //  FALSE

//! 検証用シナリオ(出力変速直進)
const scenario_running run_scenario_test_straght_NoTrace[19] = {
    {10, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {20, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {40, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {50, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {60, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {70, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {80, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {90, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {100,30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    
    {90, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {80, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {70, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {60, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {50, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {40, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {20, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {10, 15.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP}
};

#if FALSE
const scenario_running run_scenario_test_correct_straght_NoTrace[5] = {
    {50, 450.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {40, 10.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {30, 10.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {20, 10.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {10, 20.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP}
};

//! 検証用シナリオ(30m直進・ログ監視タスク評価用)
const scenario_running run_scenario_straight_30m[1] = {
    {100, 3000.0F, -1, TRACE_STRAIGHT, true,0,DISTANCE_STOP}
};

//! 検証用シナリオ(出力変速直進)真後ろ走行ver.
const scenario_running run_scenario_test_straght_NoTrace_Back[19] = {
    {-10, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-20, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-30, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-40, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-50, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-60, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-70, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-80, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-90, 30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-100,30.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    
    {-90, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-80, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-70, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-60, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-50, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-40, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-30, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-20, 15.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {-10, 15.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP}
};
#endif  //  FALSE

//! 検証用シナリオ(ゼロヨン専用)
const scenario_running run_scenario_zeroyon[2] = {
    {10, 1.0F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    {55, 450.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP}
};

#if FALSE
//! 検証用シナリオ(曲線走行90°になったら停止)
const scenario_running run_scenario_curve_90[5] = {
    {30, 0.0F, 180, NOTRACE_CURVE_LEFT, true, 40},
    {30, 0.0F, 180, NOTRACE_CURVE_LEFT, true, 30},
    {30, 0.0F, 180, NOTRACE_CURVE_LEFT, true, 20},
    {30, 0.0F, 180, NOTRACE_CURVE_LEFT, true, 10},
    {30, 0.0F, 180, NOTRACE_CURVE_LEFT, true, 5}
};

//! 検証用シナリオ(Lコースをトレースせずに走る)
const scenario_running run_scenario_LCource_Try[8] = {

    //!スタートから最初のカーブ
    {50, 246.0F, -1, NOTRACE_STRAIGHT, false,0 ,DISTANCE_STOP},
    //検証
    //{50, 64.5F, 180, NOTRACE_CURVE, false,-120,DISTANCE_STOP},
    {-30, 0.0F, -90, PINWHEEL, false,0,DISTANCE_STOP},
    {50, 41.42F, -1, NOTRACE_STRAIGHT, false,0 ,DISTANCE_STOP},
    {-30, 0.0F, -90, PINWHEEL, false,0,DISTANCE_STOP},

    //! Lコース（懸賞入口～星取り入口）
    {50, 167.7F, -1, NOTRACE_STRAIGHT, false,0,DISTANCE_STOP},
    //{50, 63.5F, -1, NOTRACE_CURVE, false,0},
    {30, 0.0F, 90, PINWHEEL, false,0,DISTANCE_STOP},
    {50, 183.1F, -1, NOTRACE_STRAIGHT, false,0 ,DISTANCE_STOP},

    {30, 167.7F, 90, PINWHEEL, true,0}
};
#endif  //  FALSE

//! 検証用(ラインを使って向きを整える)
const scenario_running correct_direction_line[3] = {
    {10, 5.0F, -1, NOTRACE_STRAIGHT, true, 0,DISTANCE_STOP},
    {10, 0.0F, -1, CORRECT_DIRECTION_BY_LINE, true, 0, DIRECTION_STOP},
    {10, 5.0F, -1, NOTRACE_STRAIGHT, true, 0,DISTANCE_STOP}
};

//! 自己位置推定Positionの検証用
const scenario_running run_scenario_test_position[5] = {
    {50, 50.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP},
    {50, 0.0F, 360, PINWHEEL, true,0, DIRECTION_STOP},
    {50, 5.0F, -1, NOTRACE_STRAIGHT, true,0,DISTANCE_STOP},
    {50, 0.0F, 90, PINWHEEL, true,0, DIRECTION_STOP},
    {10, 0.0F, 50, PINWHEEL, true,0, DIRECTION_STOP}
};

//! 検証用(ラインを使って向きを整える)
const scenario_running back[1] = {
    {-20, 30.0F, -1, NOTRACE_STRAIGHT, true, 0,DISTANCE_STOP}
};


//! 検証用シナリオ(その場360度回転・右回転)
const scenario_running run_scenario_test_pinWheel_right[1] = {
    {30, 0.0F, -360, PINWHEEL, true,0,DIRECTION_STOP}
};