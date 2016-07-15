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
 * 条件 : アームが真下を向いている状態で呼び出さなければならない
 *
 * @return  なし
*/
void initialize_arm()
{
    //! アームの基準点を設定
    sumArmMotorRotate = 0.0F;

    //! アームモーターの回転角をリセット
    ev3_motor_reset_counts(arm_motor);
}

/**
 * @brief   アームモーターを止める
 *
 * @return  ev3_motor_stop()参照
*/
ER stop_arm()
{
    ev3_speaker_stop();
    ev3_speaker_play_tone(NOTE_A4, 100);
    return ev3_motor_stop(arm_motor,true);
}

/**
 * @brief   アームを動かす
 * degreesが0～60の範囲なら安全に動作する 
 *
 * @param   degrees	回転角度，マイナスの値でモータを逆方向に回転させることができる
 * @param   speed_abs	回転速度，モータポートのフルスピードのパーセント値．範囲：0から+100．
 * @param   blocking    true (関数は回転が完了してからリターン)，false (関数は回転操作を待たずにリターン)
 *
 * @return  ev3_motor_rotate()参照
*/
ER move_arm(int degrees, uint32_t speed_abs, bool_t blocking)
{
    int armDegrees = ev3_motor_get_counts(arm_motor);
    ev3_speaker_play_tone(NOTE_A4, 100);
    
    ER result = ev3_motor_rotate(arm_motor, (degrees - armDegrees), speed_abs, blocking);
    if (result == E_OK) {
        writeFloatLCD(ev3_motor_get_counts(arm_motor));
        ev3_speaker_play_tone(NOTE_A4, 100);
    }

    return  result;
}
