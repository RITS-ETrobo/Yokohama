/**
 * @file    RunningModule.cpp
 * @brief   This file has RunningModule.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "instances.h"
#include "pid_controller.h"
#include "app.h"
#include "scenarioRunning.h"
#include "RunningModule.h"
#include "LCDController.h"
#include "Logger.h"
#include <stdlib.h>
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
    initialize_pid_controller();

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
    initialize_pid_controller();

    ev3_speaker_play_tone(NOTE_E6, 100);

    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }

    gyroSensorController->setEnabledGyroSensor(true);

    for (int index = 0; index < (int)(sizeof(run_scenario_test_straght_NoTrace) / sizeof(run_scenario_test_straght_NoTrace[0])); index++) {
        driveController->run(run_scenario_test_straght_NoTrace[index]);
    }

    ev3_speaker_play_tone(NOTE_F4, 300);
    if (logger) {
        logger->addLog(LOG_TYPE_SCENARIO, "END");
    }

    gyroSensorController->setEnabledGyroSensor(false);

}
