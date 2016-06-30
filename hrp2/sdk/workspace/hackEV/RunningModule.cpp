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

//! \addtogroup 距離計算要素
//@{
//! タイヤの直径[cm]※注意：推定した仮の値！！！
const float DiameterWheel = 7.8F;

//! 円周率
const float Pi = 3.14159265359F;
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
    int turn_ratio;
    float distance;
    bool stop;
} scenario_running;

const scenario_running run_scenario[] = {
    {100, 0, 100.0F, false},
    {50, 25, 100.0F, false},
    {50, -25, 100.0F, true}
 };

/**
 * @brief   左モータの走行距離を計算
 * 
 * @return  走行距離
*/
float distance_running(){
    float rotate = ev3_motor_get_counts(left_motor);
    float distance = 2 * Pi * DiameterWheel * rotate / 360 ;
    return distance;
}

/**
 * @brief   シナリオに従って走る
 *
 * @param   [in] scenario 走行パラメータ
 * @return  なし
*/
void run(scenario_running scenario) {
    //! 左モーターの角位置をリセット
    ev3_motor_reset_counts(left_motor);
    
    //! 走行開始
    ev3_motor_steer(left_motor, right_motor, scenario.power, scenario.turn_ratio);
    
    //! ストップ監視
    for(;;){
       //! 左モーターが指定距離走行したらストップ
       if(distance_running() >= scenario.distance){
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