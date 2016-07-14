/**
 * @file    ArmModule.cpp
 * @brief   This file has ArmModule.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "ArmModule.h"
#include "LCDController.h"
#include <stdlib.h>

//! \addtogroup 方向計算要素
//@{
//! アームモーターの回転数の合計
float sumArmMotorRotate = 0.0F;

//@}


/**
 * @brief   アームの初期化
 * 
 * @return  なし
*/
void initialize_arm() {
    //! アームの基準点を設定
    sumArmMotorRotate=0.0F;
    
    //! アームモーターの回転角をリセット
    ev3_motor_reset_counts(arm_motor);
}

/**
 * @brief   アームを動かす
 * 
 * @param アームを動かすスピード
 * @param 動かす角度
 * @return  なし
*/
void move_arm(int power, int direction) {
    ev3_motor_set_power(arm_motor, power);
    
    while(ev3_motor_get_counts(arm_motor)>=direction){
        stop_arm();
        break;
    }
}

/**
 * @brief   アームモーターを止める
 * 
 * @return  なし
*/
void stop_arm(){
    ev3_speaker_play_tone(NOTE_E6, 100);
    ev3_motor_stop(arm_motor,true);
}