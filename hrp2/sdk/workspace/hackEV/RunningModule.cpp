/**
 * @file    RunningModule.cpp
 * @brief   This file has RunningModule.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "instances.h"
#include "app.h"
#include "scenarioRunning.h"
#include "RunningModule.h"
#include "LCDController.h"
#include "Logger.h"
#include <stdlib.h>
#include "ArmModule.h"
#include "coordinateScenario.h"
#include "EV3Position.h"
#include "ArmModule.h"

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_run() {
    //! 走行パターンとPID係数のマッピング
    for (int i = TRACE_RUN_PATTARN_START; i <= TRACE_RUN_PATTARN_END; i++) {
        PID_MAP[(runPattern)i] = pidParameterList[i];
    }
}

/**
 * @brief   走る処理
 *
 * @return  なし
*/
void start_run()
{
    calibrateBW();

    ev3_speaker_play_tone(NOTE_E6, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if(ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)){
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);

    for (int index = 0; index < (int)(sizeof(L_Start_Sweepstakes_scenario) / sizeof(L_Start_Sweepstakes_scenario[0])); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        driveController->run(L_Start_Sweepstakes_scenario[index]);
    }

    gyroSensorController->setEnabledGyroSensor(false);
}

/**
 * @brief   検証用の走行
 *
 * @return  なし
*/
void start_run_test()
{
    calibrateBW();

    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);
    driveController->setEnabled();

    //! 個別のシナリオ検証用
    for (int index = 0; index < (int)(sizeof(run_scenario_test_pinWheel_right) / sizeof(run_scenario_test_pinWheel_right[0])); index++) {
        driveController->run(run_scenario_test_pinWheel_right[index]);
    }

    //! ラインを探して向きを揃える
    //driveController->catchLineAndCorrectDirection(30, 30, 30);

    ev3_speaker_play_tone(NOTE_F4, 300);
    if (logger) {
        logger->addLog(LOG_TYPE_SCENARIO, "END");
    }

    gyroSensorController->setEnabledGyroSensor(false);
    driveController->setEnabled(false);
}

/**
 * @brief   Lコース走行
 *
 * @return  なし
*/
void start_LcourseRun()
{
    calibrateBW();

    //! 【TODO】初期位置設定

    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);
    driveController->setEnabled();

    //! カーブまで移動
    for (int index = 0; index < (int)(sizeof(start_straightBeforeHoshitori) / sizeof(start_straightBeforeHoshitori[0])); index++) {
        driveController->manageMoveCoordinate(start_straightBeforeHoshitori[index]);
    }

    //! カーブで180度回転 30パワーで180度回転、曲率半径19cmで右回転
    driveController->curveOfscenario(30, 180, 19.0, NOTRACE_CURVE_RIGHT);

    //! カーブ後のストレート
    for (int index = 0; index < (int)(sizeof(straightFromCurve) / sizeof(straightFromCurve[0])); index++) {
        driveController->manageMoveCoordinate(straightFromCurve[index]);
    }

    //! カーブで80度回転 30パワーで80度回転、曲率半径19cmで左回転
    driveController->curveOfscenario(30, 80, 35.28, NOTRACE_CURVE_LEFT);


    //! カーブしてから星取りまで
    for (int index = 0; index < (int)(sizeof(toHoshitoriStraght) / sizeof(toHoshitoriStraght[0])); index++) {
        driveController->manageMoveCoordinate(toHoshitoriStraght[index]);
    }

    //! 一度この直線でラインを掴む
    //orientationPattern catchPattern = driveController->catchLineAndCorrectDirection(30, 40, 20);
    orientationPattern catchPattern = driveController->catchLine(30, 40, 20);
    //! ラインつかみに成功したらリセット
    if(catchPattern == RIGHT_PATTERN){
        //! ライン左縁を捉えている
        driveController->setNewPositionX(-860);//ここでX座標をリセット
        //driveController->setNewDirection(0.0F);//y軸に垂直になっているはずなのでリセット
    }else if(catchPattern == LEFT_PATTERN){
        //! ライン右縁を捉えている
        driveController->setNewPositionX(-853);//ここでX座標をリセット
        //driveController->setNewDirection(0.0F);//y軸に垂直になっているはずなのでリセット
    }

    //! 【TODO】灰色検知しても止まること
    for (int index = 0; index < (int)(sizeof(toGRAYBeforeHOSHI) / sizeof(toGRAYBeforeHOSHI[0])); index++) {
        driveController->manageMoveCoordinate(toGRAYBeforeHOSHI[index]);
    }

    if(driveController->foundColor){
        driveController->foundColor=false;
        driveController->setNewPositionY(1216);//ここでY座標をリセット
    }
    
    //! 星取り【色を検知しても止まること】
    for (int index = 0; index < (int)(sizeof(toHoshitori) / sizeof(toHoshitori[0])); index++) {
        driveController->manageMoveCoordinate(toHoshitori[index]);
    }

    //! 星とり付近で色を探す
    driveController->searchHOSHITORI(20, 30);
    uint8_t hoshitoriColor = driveController->colorHOSHITORI;

    //! 新幹線線前まで
    for (int index = 0; index < (int)(sizeof(beforeSUMO) / sizeof(beforeSUMO[0])); index++) {
        driveController->manageMoveCoordinate(beforeSUMO[index]);
    }

    move_arm(50, 60, false);

    //! 新幹線が目の前を通りすぎるまで待つ
    sonarSensorController->stoppingPassShinkansen();


    //! 相撲台の上に移動
    for (int index = 0; index < (int)(sizeof(onStageSumo) / sizeof(onStageSumo[0])); index++) {
        driveController->manageMoveCoordinate(onStageSumo[index]);
    }


    //! 相撲ゾーン攻略
    if(hoshitoriColor == (uint8_t)COLOR_RED){
        //! 星取りの色は赤だった場合のシナリオ
        for (int index = 0; index < (int)(sizeof(L_Sumo_scenario_hoshi_red) / sizeof(L_Sumo_scenario_hoshi_red[0])); index++) {
            driveController->run(L_Sumo_scenario_hoshi_red[index]);
        }
    }
    else if(hoshitoriColor == (uint8_t)COLOR_BLUE){
        //! 星取りの色は青だった場合のシナリオ
        for (int index = 0; index < (int)(sizeof(L_Sumo_scenario_hoshi_blue) / sizeof(L_Sumo_scenario_hoshi_blue[0])); index++) {
            driveController->run(L_Sumo_scenario_hoshi_blue[index]);
        }
    }
    else if(hoshitoriColor == (uint8_t)COLOR_GREEN){
        //! 星取りの色は緑だった場合のシナリオ
        for (int index = 0; index < (int)(sizeof(L_Sumo_scenario_hoshi_green) / sizeof(L_Sumo_scenario_hoshi_green[0])); index++) {
            driveController->run(L_Sumo_scenario_hoshi_green[index]);
        }
    }
    else{
        //! 星取りの色は黄色だった場合のシナリオ
        for (int index = 0; index < (int)(sizeof(L_Sumo_scenario_hoshi_yellow) / sizeof(L_Sumo_scenario_hoshi_yellow[0])); index++) {
            driveController->run(L_Sumo_scenario_hoshi_yellow[index]);
        }
    }
    move_arm(0, 60, true);
    

    //! 相撲が終わったら新幹線が目の前を通りすぎるまで待つ
    sonarSensorController->stoppingPassShinkansen();

    //! 相撲ゾーン終わった位置から懸賞前の直線
    for (int index = 0; index < (int)(sizeof(fromSumo_toKenshoStraight) / sizeof(fromSumo_toKenshoStraight[0])); index++) {
        driveController->manageMoveCoordinate(fromSumo_toKenshoStraight[index]);
    }

    catchPattern = driveController->catchLineRIGHT(30, 35, 20);
    //! ラインつかみに成功したらリセット
    if(catchPattern == RIGHT_PATTERN){
        //! ライン左縁を捉えている
        driveController->setNewPositionX(-217);//ここでX座標をリセット
        driveController->setNewDirection(0.0F);//y軸に垂直になっているはずなのでリセット
    }else if(catchPattern == LEFT_PATTERN){
        //! ライン右縁を捉えている
        driveController->setNewPositionX(-211);//ここでX座標をリセット
        driveController->setNewDirection(0.0F);//y軸に垂直になっているはずなのでリセット
    }


    //! 懸賞前の灰色
    for (int index = 0; index < (int)(sizeof(toGRAYBeforeKENSHO) / sizeof(toGRAYBeforeKENSHO[0])); index++) {
        driveController->manageMoveCoordinate(toGRAYBeforeKENSHO[index]);
    }

    if(driveController->foundColor){
        driveController->foundColor=false;
        driveController->setNewPositionY(1216);//ここでY座標をリセット
    }

    // for (int index = 0; index < (int)(sizeof(KENSHO) / sizeof(KENSHO[0])); index++) {
    //     driveController->manageMoveCoordinate(KENSHO[index]);
    // }

    //! 【TODO】懸賞をとる作業
    //move_arm(80, 100, true);

    //! 懸賞からゴールまで
    for (int index = 0; index < (int)(sizeof(fromKensho_toGoal) / sizeof(fromKensho_toGoal[0])); index++) {
        driveController->manageMoveCoordinate(fromKensho_toGoal[index]);
    }


    ev3_speaker_play_tone(NOTE_F4, 300);
    if (logger) {
        logger->addLog(LOG_TYPE_SCENARIO, "END");
    }

    gyroSensorController->setEnabledGyroSensor(false);
    driveController->setEnabled(false);
}


/**
 * @brief   Rコース走行
 *
 * @return  なし
*/
void start_RcourseRun()
{
    calibrateBW();

    //! 【TODO】初期位置設定

    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);
    driveController->setEnabled();



    //! 格子前まで移動
    for (int index = 0; index < (int)(sizeof(toBeforeKOUSHI) / sizeof(toBeforeKOUSHI[0])); index++) {
        driveController->manageMoveCoordinate(toBeforeKOUSHI[index]);
    }

    //! カーブで90度回転 30パワーで80度回転、曲率半径19cmで左回転
    driveController->curveOfscenario(30, 92, 13.14, NOTRACE_CURVE_LEFT);
    
    //! 青攻略
    BuleRootTry();

    //! ショートカットしないルート
    for (int index = 0; index < (int)(sizeof(NoShortcutRoot) / sizeof(NoShortcutRoot[0])); index++) {
        driveController->manageMoveCoordinate(NoShortcutRoot[index]);
    }

    ev3_speaker_play_tone(NOTE_F4, 300);
    if (logger) {
        logger->addLog(LOG_TYPE_SCENARIO, "END");
    }

    gyroSensorController->setEnabledGyroSensor(false);
    driveController->setEnabled(false);
}

void start_RcourseRunShortCut(){
    calibrateBW();

    //! 【TODO】初期位置設定

    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);
    driveController->setEnabled();

    //! 格子前まで移動
    for (int index = 0; index < (int)(sizeof(toBeforeKOUSHI) / sizeof(toBeforeKOUSHI[0])); index++) {
        driveController->manageMoveCoordinate(toBeforeKOUSHI[index]);
    }

    //! 青攻略
    BuleRootTry();

    //! ショートカットするルート
    for (int index = 0; index < (int)(sizeof(YesShortcutRoot) / sizeof(YesShortcutRoot[0])); index++) {
        driveController->manageMoveCoordinate(YesShortcutRoot[index]);
    }

    ev3_speaker_play_tone(NOTE_F4, 300);
    if (logger) {
        logger->addLog(LOG_TYPE_SCENARIO, "END");
    }

    gyroSensorController->setEnabledGyroSensor(false);
    driveController->setEnabled(false);
}

//! 青攻略
void BuleRootTry(){
    //! 【TODO】格子の中を攻略
    for (int index = 0; index < (int)(sizeof(BlueRoot) / sizeof(BlueRoot[0])); index++) {
        driveController->manageMoveCoordinate(BlueRoot[index]);
    }

    //! 後ろに戻る
    for (int index = 0; index < (int)(sizeof(back) / sizeof(back[0])); index++) {
        driveController->run(back[index]);
    }

        //! 格子を抜ける
    for (int index = 0; index < (int)(sizeof(nigeru) / sizeof(nigeru[0])); index++) {
        driveController->manageMoveCoordinate(nigeru[index]);
    }
}