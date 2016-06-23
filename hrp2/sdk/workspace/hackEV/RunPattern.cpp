/**
 * @file    UltrasonicController.cpp
 * @brief   This file has UltrasonicController.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "RunPattern.h"


/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_run() {
    
}

/**
 * @brief   直線を走る処理
 *
 * @param   [in] power 走行出力
 * @return  なし
*/
void straight_run(int power)
{
    while(1){
        if(ev3_touch_sensor_is_pressed(touch_sensor)){
            break;
        }
    }
    ev3_motor_steer(left_motor, right_motor, 100, 0);
}

/**
 * @brief   カーブを走る処理
 *
 * @param   [in] power 走行出力
 * @param   [in] corveAngle カーブの角度
 * @return  なし
*/
void curve_run(int power,int corveAngle){

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