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
    for (int index = 0; index < (int)(sizeof(run_scenario_test_position) / sizeof(run_scenario_test_position[0])); index++) {
        driveController->run(run_scenario_test_position[index]);
    }

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

    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);
    driveController->setEnabled();

    //! 相撲前まで座標移動
    for (int index = 0; index < (int)(sizeof(start_beforeSumo) / sizeof(start_beforeSumo[0])); index++) {
        driveController->manageMoveCoordinate(start_beforeSumo[index]);
    }

    //! 星とり付近で色を探す
     driveController->searchHOSHITORI(20, 30);

    //! 【TODO】新幹線処理

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

    //! 相撲ゾーン終わった位置から懸賞まで
    for (int index = 0; index < (int)(sizeof(fromSumo_toKensho) / sizeof(fromSumo_toKensho[0])); index++) {
        driveController->manageMoveCoordinate(fromSumo_toKensho[index]);
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
