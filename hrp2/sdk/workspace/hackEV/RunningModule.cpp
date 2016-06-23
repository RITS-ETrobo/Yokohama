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


/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_run() {
    
}

/**
 * @brief   走る処理
 *
 * @param   [in] power 走行出力
 * @param   [in] corveAngle カーブの角度
 * @return  なし
*/
void start_run(int power,int corveAngle){
    //! 直進モードの準備ができたら音が3回鳴る
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_C4, 100);
    
    while(1){
        if(ev3_touch_sensor_is_pressed(touch_sensor)){
            break;
        }
    }
    //! 走行処理（注意：走行中断ができない）
    ev3_motor_steer(left_motor, right_motor, power, corveAngle);
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