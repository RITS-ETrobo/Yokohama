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
#include <map>

//! \addtogroup 方向計算要素
//@{
//! 向きの累積[単位 : 度]
float directionTotal = 0.0F;

//! 距離の累積[単位 : cm]
float distanceTotal = 0.0F;
//@}

std::map<runPattern, PID_PARAMETER> PID_MAP;

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
 * @brief   瞬間の走行体の向きと走行体中心の移動距離を取得する
 * 「瞬間の走行体の向き」とは、前回測定した位置から今回の移動までに変化した向きである
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  なし
*/
void getDelta(float *directionDelta, float *distanceDelta)
{
    float   distanceDeltaLeft = motorWheelLeft->getDistanceDelta();
    float   distanceDeltaRight = motorWheelRight->getDistanceDelta();
    *directionDelta = ((distanceDeltaRight - distanceDeltaLeft) / EV3_TREAD) * 180 / Pi;
    *distanceDelta = (distanceDeltaRight + distanceDeltaLeft) / 2.0F;
}

/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  走行距離[単位 : cm]
*/
float getDistance(float distanceDelta)
{
    distanceTotal += distanceDelta;

    if (logger) {
        //! 距離をログ出力
        char message[16];
        memset(message, '\0', sizeof(message));
        sprintf(message, "%02.04f",distanceTotal);
        logger->addLog(LOG_TYPE_DISTANCE_TOTAL, message);
    }

    return  distanceTotal;
}

/**
 * @brief   リセットしてからの走行体の向きを取得する
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @return  走行体の向き[単位 : 度]
*/
float getDirection(float directionDelta)
{
    directionTotal += directionDelta;
    if (logger) {
        char message[16];
        memset(message, '\0', sizeof(message));
        sprintf(message, "%02.04f",directionTotal);
        logger->addLog(LOG_TYPE_DIRECTION_TOTAL, message);
    }

    return  directionTotal;
}

/**
 * @brief   ストップ処理
 * 
 * @return  なし
*/
void stop_run()
{
    ev3_speaker_play_tone(NOTE_E6, 100);
    motorWheelLeft->stop();
    motorWheelRight->stop();
}

/**
 * @brief   その場回転
 * 
 * @return  なし
 */
void pinWheel(int power){
    ev3_motor_set_power(EV3_MOTOR_LEFT, (-power));
    ev3_motor_set_power(EV3_MOTOR_RIGHT, power);
}

/**
 * @brief   モーターの回転角、距離、方向を0に戻す
 *
 * @return  なし
*/
void initialize_wheel()
{
    //! モーターの角位置、向きの累積をリセット
    directionTotal = 0.0F;
    distanceTotal = 0.0F;
    motorWheelLeft->initialize();
    motorWheelRight->initialize();
}

/**
 * @brief   ラインの縁の変更処理
 * 
 * @enum LineSide targetSide 移動させたい縁
 * @return  なし
 */
void change_LineSide(scenario_running scenario)
{   
    //! 最初に回転するタイヤ
    motor_port_t firstMoveWheel;

    //! 次に回転するタイヤ
    motor_port_t secondMoveWheel;
    
    switch (scenario.pattern) {
        case SWITCH_SIDE_LEFT:
            //! 移動したい縁が左のときは最初に右タイヤ、次に左タイヤを旋回させて切り替え
            firstMoveWheel = EV3_MOTOR_RIGHT;
            secondMoveWheel = EV3_MOTOR_LEFT;
            break;
            
        case SWITCH_SIDE_RIGHT:
            //! 移動したい縁が右のときは最初に左タイヤ、次に右タイヤを旋回させて切り替え
            firstMoveWheel = EV3_MOTOR_LEFT;
            secondMoveWheel = EV3_MOTOR_RIGHT;
            break;
            
        default:
            break;
    }
    
    //! ラインの黒線の上にいるフラグ（黒線をまたいで移動するため）
    bool onBlack = false;
    
    //! 片方だけのタイヤを反対側まで旋回
    ev3_motor_set_power(firstMoveWheel, scenario.power);
    float firstDirection = 0.0F;
    float   directionDelta = 0.0F;
    float   distanceDelta = 0.0F;
    for(;;){
        //! 動いた角度を記録
        getDelta(&directionDelta, &distanceDelta);
        firstDirection += directionDelta; 

        int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
        if(colorValue < (black + 5)){
            onBlack = true;
        }
        
        if(colorValue > (white - 5) && onBlack){
            ev3_motor_stop(firstMoveWheel,true);
            writeFloatLCD((float)colorValue);
            break;
        }
    }
    
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    
    //! 目的の縁まで回転させたらその場回転で向きを戻す
    //ev3_motor_set_power(firstMoveWheel, (power/2));
    ev3_motor_set_power(secondMoveWheel, scenario.power);
    float secondDirection=0.0F;
    for(;;){
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        getDelta(&directionDelta, &distanceDelta);
        secondDirection += directionDelta; 

        //! 走行体が最初に動いた角度分戻ったらストップ
        if(abs(secondDirection) >= abs(firstDirection)){
            //writeFloatLCD((float)colorValue);
            stop_run();
            break;
        }
    } 
}

/**
 * @brief   シナリオに従って走る
 *
 * @param   [in] scenario 走行パラメータ
 * @return  なし
*/
void run(scenario_running scenario)
{
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    float   distanceDelta = 0.0F;
    float   directionDelta = 0.0F;

    //! ストップ監視しつつ、走行
    for (;;) {
        //! 走行
        switch (scenario.pattern) {
	    case PINWHEEL:
            //! その場回転
            pinWheel(scenario.power);
            break;

        case NOTRACE_STRAIGHT:
            //! ライントレースせずに、直進走行する
            ev3_motor_set_power(EV3_MOTOR_LEFT, scenario.power);
            ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
            break;
            
        case SWITCH_SIDE_RIGHT:
        case SWITCH_SIDE_LEFT:
            //! ライントレースする縁を変更
            change_LineSide(scenario);
            return;
            
        case ONESIDE_PINWHEEL_RIGHT:
            //! 右回転(左タイヤのみ回転)
            ev3_motor_set_power(EV3_MOTOR_LEFT, scenario.power);
            motorWheelRight->stop();
            break;
            
        case ONESIDE_PINWHEEL_LEFT:
            //! 左回転(右タイヤのみ回転)
            ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
            motorWheelLeft->stop();
            break;
        
        case TRACE_STRAIGHT_RIGHT:
        case TRACE_CURVE_RIGHT:
        {
            //! ライン右側をトレース
            float pidValueForRight = (-pid_controller(PID_MAP[scenario.pattern]));
            ev3_motor_steer(EV3_MOTOR_LEFT, EV3_MOTOR_RIGHT, scenario.power, pidValueForRight);
        }
        break;

        default:
            //! ライン左側をトレース
            ev3_motor_steer(EV3_MOTOR_LEFT, EV3_MOTOR_RIGHT, scenario.power, pid_controller(PID_MAP[scenario.pattern]));
            break;
        }
        
        tslp_tsk(1);//この行の必要性については要検証

        getDelta(&directionDelta, &distanceDelta);

        //! 距離判定の必要性判断
        if (scenario.distance != 0) {
            getDistance(distanceDelta);

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
        getDirection(directionDelta);

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
