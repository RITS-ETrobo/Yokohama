/**
 * @file    RunningModule.cpp
 * @brief   This file has RunningModule.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "RunningModule.h"
#include "LCDController.h"
#include <stdlib.h>
#include "ArmModule.h"

//! \addtogroup 距離計算要素
//@{
//! タイヤの直径[cm]
const float DiameterWheel = 8.2F;

//! 円周率
const float Pi = 3.14159265359F;
//@}

//! \addtogroup 方向計算要素
//@{
//! 左モーターの回転数の合計
float sumLeftMotorRotate = 0.0F;

//! 右モーターの回転数の合計
float sumRightMotorRotate = 0.0F;

//! 左右のタイヤ間のトレッド[cm]
const float TREAD = 12.7F;

//! 前回の右モーターの距離
float lastRightDistance = 0.0F;

//! 前回の左モーターの距離
float lastLeftDistance = 0.0F;

//! 向きの累積
float directionSum = 0.0F;
//@}

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_run() {
    
}


typedef struct{
    int power;
    float distance;
    int direction;
    bool pinwheel;
    bool stop;
    PID_PARAMETER pidParameter;
} scenario_running;

const scenario_running run_scenario[] = {
    {30, 100.0F, 90, false, false, {0.775F, 0.0F, 0.375F}},
    {30, 100.0F, 90, false, false, {0.775F, 0.65F, 0.375F}},
    {30, 100.0F, 90, false, true, {0.775F, 0.65F, 0.375F}}
 };
 
const scenario_running run_scenario_test[] = {
    {30, 42.0F, 180, true, false, {0.775F, 0.0F, 0.375F}},
    {-30, 42.0F, -180, true, false, {0.775F, 0.65F, 0.375F}},
    {30, 42.0F, 180, false, false, {0.775F, 0.90F, 0.375F}},
    {30, 42.0F, 200, true, false, {0.775F, 0.65F, 0.375F}},
    {30, 42.0F, 180, false, false, {0.775F, 0.90F, 0.375F}},
    {30, 42.0F, 400, true, true, {0.775F, 0.65F, 0.375F}}
 };

/**
 * @brief   リセットしてからの指定したタイヤの走行距離を計算
 * 
 * @param port 計測するタイヤのモーターポート
 * @return  走行距離
*/
float distance_running(motor_port_t port){
    float rotate = ev3_motor_get_counts(port);
    float distance = Pi * DiameterWheel * rotate / 360 ;
    return distance;
}

/**
 * @brief   瞬間の走行体の向きを取得
 * 前回測定した位置から今回の移動までに変化した向き
 * 
 * @return  瞬間の向き[度]
*/
float getDirectionDelta(float rightDistance, float leftDistance){
    
    float rightDistanceDelta = rightDistance - lastRightDistance;
    float leftDistanceDelta = leftDistance - lastLeftDistance;
    lastRightDistance = rightDistance;
    lastLeftDistance = leftDistance;
    
    //! 走行体の向き[度]
    float direction = ((rightDistanceDelta - leftDistanceDelta) / TREAD) * 180 / Pi;
    
    return direction;
}

/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * 
 * @return  走行距離[cm]
*/
float getDistance(float rightDistance, float leftDistance){
    float distance = (rightDistance + leftDistance) / 2.0F;
    return distance;
}

/**
 * @brief   ストップ処理
 * 
 * @return  なし
*/
void stop_run(){
    ev3_speaker_play_tone(NOTE_E6, 100);
    ev3_motor_stop(left_motor,true);
    ev3_motor_stop(right_motor,true); 
}

/**
 * @brief   シナリオに従って走る
 *
 * @param   [in] scenario 走行パラメータ
 * @return  なし
*/
void run(scenario_running scenario) {
    //! モーターの角位置、向きの累積をリセット
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    lastRightDistance = 0.0F;
    lastLeftDistance = 0.0F;
    directionSum = 0.0F;
    
    //! ストップ監視しつつ、走行
    for(;;){
        if(scenario.pinwheel){
            //! その場回転
            ev3_motor_set_power(left_motor, (-scenario.power));
            ev3_motor_set_power(right_motor, scenario.power);
        }else{
            //! PIDを用いた走行
            ev3_motor_steer(left_motor, right_motor, scenario.power, pid_controller(scenario.pidParameter));
        }
        
        tslp_tsk(1);//この行の必要性については要検証
        
        //! 現在の左と右のモーターの走行距離を取得
        float leftDistance = distance_running(left_motor);
        float rightDistance = distance_running(right_motor);
        
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        directionSum += getDirectionDelta(rightDistance, leftDistance);   
        
        //! 走行体が指定距離走行したらストップ
        if(getDistance(rightDistance, leftDistance) >= scenario.distance){
            if(scenario.stop){
                stop_run();
                
                //! 左モーターの移動距離結果をLCDに表示
                writeFloatLCD(leftDistance);
                
                //! 右モーターの移動距離結果をLCDに表示
                writeFloatLCD(rightDistance);
            }

            break;
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

/**
 * @brief   走る処理
 *
 * @return  なし
*/
void start_run(){
    initialize_pid_controller();
    
    for(int index = 0; index < sizeof(run_scenario) / sizeof(run_scenario[0]); index++ ){
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario[index]);
    }
}

/**
 * @brief   検証用の走行
 *
 * @param   なし    
 * @return  なし
*/
void start_run_test(){
    initialize_pid_controller();
    
    ev3_speaker_play_tone(NOTE_E6, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);
    
    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    while(1){
        if(ev3_touch_sensor_is_pressed(touch_sensor)){
            break;
        }
    }
    
    for(int index = 0; index < sizeof(run_scenario_test) / sizeof(run_scenario_test[0]); index++ ){
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario_test[index]);
    }

}