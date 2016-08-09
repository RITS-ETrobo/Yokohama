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

//! \addtogroup 距離計算要素
//@{
//! 円周率
const float Pi = 3.14159265359F;
//@}

//! \addtogroup 方向計算要素
//@{
//! 左モーターの回転数の合計
float sumLeftMotorRotate = 0.0F;

//! 右モーターの回転数の合計
float sumRightMotorRotate = 0.0F;

//! 前回の右モーターの距離[cm]
float lastRightDistance = 0.0F;

//! 前回の左モーターの距離[cm]
float lastLeftDistance = 0.0F;

//! 向きの累積[度]
float directionSum = 0.0F;
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
 * @brief   リセットしてからの指定したタイヤの走行距離を計算する
 * 
 * @param   port    計測するタイヤのモーターポート
 * @return  走行距離[cm]
*/
float distance_running(motor_port_t port)
{
    float distance = Pi * EV3_WHEEL_DIAMETER * ev3_motor_get_counts(port) / 360;
    return distance;
}

/**
 * @brief   瞬間の走行体の向きを取得する
 * 「瞬間の走行体の向き」とは、前回測定した位置から今回の移動までに変化した向きである
 * @param rightDistance 右タイヤの走行距離[cm]
 * @param leftDistance  左タイヤの走行距離[cm]
 * @return  瞬間の向き[度]
*/
float getDirectionDelta(float rightDistance, float leftDistance)
{
    float rightDistanceDelta = rightDistance - lastRightDistance;
    float leftDistanceDelta = leftDistance - lastLeftDistance;
    lastRightDistance = rightDistance;
    lastLeftDistance = leftDistance;
    
    //! 走行体の向き[度]
    float direction = ((rightDistanceDelta - leftDistanceDelta) / EV3_TREAD) * 180 / Pi;
    return direction;
}

/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * 
 * @return  走行距離[cm]
*/
float getDistance(float rightDistance, float leftDistance)
{
    float distance = (rightDistance + leftDistance) / 2.0F;
    return distance;
}

/**
 * @brief   ストップ処理
 * 
 * @return  なし
*/
void stop_run()
{
    ev3_speaker_play_tone(NOTE_E6, 100);
    ev3_motor_stop(EV3_MOTOR_LEFT,true);
    ev3_motor_stop(EV3_MOTOR_RIGHT,true); 
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
void initialize_wheel(){
    //! モーターの角位置、向きの累積をリセット
    ev3_motor_reset_counts(EV3_MOTOR_LEFT);
    ev3_motor_reset_counts(EV3_MOTOR_RIGHT);
    lastRightDistance = 0.0F;
    lastLeftDistance = 0.0F;
    directionSum = 0.0F;
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
    for(;;){
        //! 動いた角度を記録
        firstDirection += getDirectionDelta(distance_running(EV3_MOTOR_LEFT), distance_running(EV3_MOTOR_RIGHT)); 
 
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
        secondDirection += getDirectionDelta(distance_running(EV3_MOTOR_LEFT), distance_running(EV3_MOTOR_RIGHT)); 
        
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
             ev3_motor_stop(EV3_MOTOR_RIGHT,true);
            break;
            
        case ONESIDE_PINWHEEL_LEFT:
            //! 左回転(右タイヤのみ回転)
            ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
            ev3_motor_stop(EV3_MOTOR_LEFT,true);
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
        
        //! 現在の左と右のモーターの走行距離を取得
        float leftDistance = distance_running(EV3_MOTOR_LEFT);
        float rightDistance = distance_running(EV3_MOTOR_RIGHT);      
        
        char message[16];
        
        //! 距離判定の必要性判断
        if (scenario.distance != 0) {
            float sumDistance = getDistance(rightDistance, leftDistance);
            
            //! 距離をログ出力
            memset(message, '\0', sizeof(message));
            sprintf(message, "%02.04f",sumDistance);
            if (logger) {
                logger->addLog(LOG_TYPE_DISTANCE, message);
            }
            
            //! 走行体が指定距離走行したらストップ
            if(abs(sumDistance) >= abs(scenario.distance)){
                if(scenario.stop){
                    stop_run();
                    
                    //! 左モーターの移動距離結果をLCDに表示
                    writeFloatLCD(leftDistance);
                    
                    //! 右モーターの移動距離結果をLCDに表示
                    writeFloatLCD(rightDistance);
                }

                break;
            }
        }
        
        //! 方向判定の必要性判断
        if (scenario.direction != -1) {
            //! 瞬間の向きを取得
            float directionDelta = getDirectionDelta(rightDistance, leftDistance);
            memset(message, '\0', sizeof(message));
            sprintf(message, "%02.04f",directionDelta);
            if (logger) {
                logger->addLog(LOG_TYPE_DIRECTION, message);
            }
            
            //! 累積して走行体の向きを計測
            directionSum += directionDelta;
            memset(message, '\0', sizeof(message));
            sprintf(message, "%02.04f",directionSum);
            if (logger) {
                logger->addLog(LOG_TYPE_DIRECTION_STORED, message);
            }

            //! 走行体が指定した向きになったらストップ
            if(abs(directionSum) >= abs(scenario.direction)){
                if(scenario.stop){
                    stop_run();
                    
                    //! 走行体の向きをLCDに表示
                    writeFloatLCD(directionSum);                
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

/**
 * @brief   新幹線まで走る
 *
 * @return  なし
*/
void start_run_test_foward()
{
    ev3_speaker_play_tone(NOTE_E6, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);
    
    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }
    
    for (int index = 0; index < sizeof(run_scenario_test_straght_NoTrace) / sizeof(run_scenario_test_straght_NoTrace[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario_test_straght_NoTrace[index]);
    }
}

/**
 * @brief   新幹線の位置からもどる
 *
 * @return  なし
*/
void start_run_test_back()
{
    ev3_speaker_play_tone(NOTE_E6, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);
    
    for (int index = 0; index < sizeof(run_scenario_test_straght_NoTrace_back) / sizeof(run_scenario_test_straght_NoTrace_back[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario_test_straght_NoTrace_back[index]);
    }
}