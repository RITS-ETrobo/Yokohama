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
    // for (int index = 0; index < (int)(sizeof(correct_direction_line) / sizeof(correct_direction_line[0])); index++) {
    //     driveController->run(correct_direction_line[index]);
    // }

    //! ラインを探して向きを揃える
    driveController->catchLineAndCorrectDirection(30, 30, 30);

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

    //! 星取り前の直線まで移動
    for (int index = 0; index < (int)(sizeof(start_straightBeforeHoshitori) / sizeof(start_straightBeforeHoshitori[0])); index++) {
        driveController->manageMoveCoordinate(start_straightBeforeHoshitori[index]);
    }

    //! 一度この直線でラインを掴む
    orientationPattern catchPattern = driveController->catchLineAndCorrectDirection(30, 40, 20);
    //! ラインつかみに成功したらリセット
    if(catchPattern == RIGHT_PATTERN){
        //! ライン左縁を捉えている
        driveController->setNewPositionX(-860);//ここでX座標をリセット
        driveController->setNewDirection(0.0F);//y軸に垂直になっているはずなのでリセット
    }else if(catchPattern == LEFT_PATTERN){
        //! ライン右縁を捉えている
        driveController->setNewPositionX(-853);//ここでX座標をリセット
        driveController->setNewDirection(0.0F);//y軸に垂直になっているはずなのでリセット
    }

    //! 【TODO】灰色検知しても止まること
    for (int index = 0; index < (int)(sizeof(toGRAYBeforeHOSHI) / sizeof(toGRAYBeforeHOSHI[0])); index++) {
        driveController->manageMoveCoordinate(toGRAYBeforeHOSHI[index]);
    }

    driveController->
    
    //! 星取り【色を検知しても止まること】
    for (int index = 0; index < (int)(sizeof(toHoshitori) / sizeof(toHoshitori[0])); index++) {
        driveController->manageMoveCoordinate(toHoshitori[index]);
    }

    //! 【TODO】もし色をさがせてなければ色を探す

    //! 新幹線線前まで
    for (int index = 0; index < (int)(sizeof(beforeSUMO) / sizeof(beforeSUMO[0])); index++) {
        driveController->manageMoveCoordinate(beforeSUMO[index]);
    }

    //! 新幹線が目の前を通りすぎるまで待つ
    sonarSensorController->stoppingPassShinkansen();

    //! 相撲台の上に移動
    for (int index = 0; index < (int)(sizeof(onStageSumo) / sizeof(onStageSumo[0])); index++) {
        driveController->manageMoveCoordinate(onStageSumo[index]);
    }

    //! 星取りカラー【TODO】取得した色を取得するようにする
    int hoshitoriColor = 0;

    //! 相撲ゾーン攻略
    if(hoshitoriColor == 0){
        //! 星取りの色は赤だった場合のシナリオ
        for (int index = 0; index < (int)(sizeof(L_Sumo_scenario_hoshi_red) / sizeof(L_Sumo_scenario_hoshi_red[0])); index++) {
            driveController->run(L_Sumo_scenario_hoshi_red[index]);
        }
    }
    else if(hoshitoriColor == 1){

    }
    else if(hoshitoriColor == 2){

    }
    else{

    }

    //! 相撲が終わったら新幹線が目の前を通りすぎるまで待つ
    sonarSensorController->stoppingPassShinkansen();

    //! 相撲ゾーン終わった位置から懸賞前の直線
    for (int index = 0; index < (int)(sizeof(fromSumo_toKenshoStraight) / sizeof(fromSumo_toKenshoStraight[0])); index++) {
        driveController->manageMoveCoordinate(fromSumo_toKenshoStraight[index]);
    }

    catchPattern = driveController->catchLineAndCorrectDirection(30, 40, 20);
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

    for (int index = 0; index < (int)(sizeof(KENSHO) / sizeof(KENSHO[0])); index++) {
        driveController->manageMoveCoordinate(KENSHO[index]);
    }

    //! 【TODO】懸賞をとる作業

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
