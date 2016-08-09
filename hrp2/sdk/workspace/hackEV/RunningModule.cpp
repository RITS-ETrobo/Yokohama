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
 * @brief   ストップ処理
 * 
 * @return  なし
*/
void stop_run()
{
    ev3_speaker_play_tone(NOTE_E6, 100);
    driveController->stop();
}

/**
 * @brief   シナリオに従って走る
 *
 * @param   [in] scenario 走行パラメータ
 * @return  なし
*/
void run(scenario_running scenario)
{
    //! モーターの回転角、距離、方向を0に戻す
    driveController->initialize();

    float   distanceDelta = 0.0F;
    float   directionDelta = 0.0F;

    //! ストップ監視しつつ、走行
    for (;;) {
        //! 走行
        if (driveController->runAsPattern(scenario)) {
            return;
        }

        tslp_tsk(1);//この行の必要性については要検証

        driveController->getDelta(&directionDelta, &distanceDelta);

        //! 距離判定の必要性判断
        if (scenario.distance != 0) {
            float   distanceTotal = driveController->getDistance(distanceDelta);

            //! 走行体が指定距離走行したらストップ
            if(abs(distanceTotal) >= abs(scenario.distance)){
                if(scenario.stop){
                    stop_run();
                }

                break;
            }
        }    
        
        //! 瞬間の向きを取得
        char message[16];
        memset(message, '\0', sizeof(message));
        sprintf(message, "%02.04f",directionDelta);
        if (logger) {
            logger->addLog(LOG_TYPE_DIRECTION, message);
        }
        
        //! 累積して走行体の向きを計測
        float   directionTotal = driveController->getDirection(directionDelta);

        //! 方向判定の必要性判断
        if (scenario.direction != -1) {
            //! 走行体が指定した向きになったらストップ
            if(abs(directionTotal) >= abs(scenario.direction)){
                if(scenario.stop){
                    stop_run();
                    
                    //! 走行体の向きをLCDに表示
                    writeFloatLCD(directionTotal);                
                }

                break;
            }
        }
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

    for (int index = 0; index < sizeof(L_Start_Sweepstakes_scenario) / sizeof(L_Start_Sweepstakes_scenario[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(L_Start_Sweepstakes_scenario[index]);
    }
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
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);
    
    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }
    
    for (int index = 0; index < sizeof(run_scenario_test_pinWheel) / sizeof(run_scenario_test_pinWheel[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario_test_pinWheel[index]);
    }
}
