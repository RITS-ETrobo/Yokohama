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
const float TREAD = 10.9F;

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
    bool stop;
    PID_PARAMETER pidParameter;
} scenario_running;

const scenario_running run_scenario[] = {
    {30, 100.0F, false, {0.775F, 0.0F, 0.375F}},
    {30, 100.0F, false, {0.775F, 0.65F, 0.375F}},
    {30, 100.0F, true, {0.775F, 0.65F, 0.375F}}
 };

/**
 * @brief   リセットしてからのタイヤの走行距離を計算
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
 * @return  瞬間の向き
*/
float getDirectionDelta(float rightDistance, float leftDistance){
    
    float rightDistanceDelta = rightDistance - lastRightDistance;
    float leftDistanceDelta = leftDistance - lastLeftDistance;
    lastRightDistance = rightDistance;
    lastLeftDistance = leftDistance;

    //! 走行体の向き
    float direction = (leftDistanceDelta - rightDistanceDelta) / TREAD ;
    return direction;
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
        ev3_motor_steer(left_motor, right_motor, scenario.power, pid_controller(scenario.pidParameter));
        tslp_tsk(1);//この行の必要性については要検証
        
        //! 現在の左と右のモーターの走行距離を取得
        float leftDistance = distance_running(left_motor);
        float rightDistance = distance_running(right_motor);
        
        //! 瞬間の向きを取得し、累積する
        directionSum += getDirectionDelta(rightDistance, leftDistance);
        
        //! 向きの累積をLCDに表示
        char message[16];
        memset(message, '\0', sizeof(message));
        sprintf(message, "%03.03f", directionSum); 
        writeStringLCD(message);
        
        //! 左モーターが指定距離走行したらストップ
        if(leftDistance >= scenario.distance){
            if(scenario.stop){
                ev3_motor_stop(left_motor,false);
                ev3_motor_stop(right_motor,false);
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
        run(run_scenario[index]);
    }
}

/**
 * @brief   補正しながら走行
 * 
 * PID係数を調節しながら走行する
 *
 * @param   なし    
 * @return  なし
*/
void run_withPID(){
    

}