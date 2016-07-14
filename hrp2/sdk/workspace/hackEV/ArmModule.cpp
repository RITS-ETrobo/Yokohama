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
 * 条件：アームが真下を向いている状態で呼び出さなければならない
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
 * @brief   アームモーターを止める
 * 
 * @return  なし
*/
void stop_arm(){
    ev3_speaker_stop();
    ev3_speaker_play_tone(NOTE_A4, 100);
    ER er = ev3_motor_stop(arm_motor,true);
    if(er != E_OK){    
        return;
    }
}

/**
 * @brief   アームを動かす
 * 0から60までの位置なら安全に動作する 
 *
 * @param アームを動かすスピード
 * @param 角位置
 * @return  なし
*/
void move_arm(int power, int direction, bool bloking) {
    int armDirection = ev3_motor_get_counts(arm_motor);
    ev3_speaker_play_tone(NOTE_A4, 100);
    
    ER er = ev3_motor_rotate(arm_motor, (direction-armDirection), power, bloking);
    if(er != E_OK){
        return;
    }
    
    writeFloatLCD(ev3_motor_get_counts(arm_motor));
    ev3_speaker_play_tone(NOTE_A4, 100);
}

