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

//! 下向き側の駆動域の限界から何度戻したところを初期位置とするか(実際に動かす際には、この値はinitialize_armでリセットされているため考慮する必要はない)
int initializePosition = 10;

//@}


/**
 * @brief   アームの初期化
 * 条件 : アームが真下を向いている状態で呼び出さなければならない
 *
 * @return  なし
*/
void initialize_arm()
{
    //! アームの初期位置まで動かす
    initialize_arm_position();

    //! アームの基準点を設定
    sumArmMotorRotate = 0.0F;

    //! アームモーターの回転角をリセット
    ev3_motor_reset_counts(EV3_MOTOR_ARM);
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
    return ev3_motor_stop(EV3_MOTOR_ARM,true);
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
    int armDegrees = ev3_motor_get_counts(EV3_MOTOR_ARM);
    ev3_speaker_play_tone(NOTE_A4, 100);
    
    ER result = ev3_motor_rotate(EV3_MOTOR_ARM, (degrees - armDegrees), speed_abs, blocking);
    if (result == E_OK) {
        writeFloatLCD(ev3_motor_get_counts(EV3_MOTOR_ARM));
        ev3_speaker_play_tone(NOTE_A4, 100);
    }

    return  result;
}

/**
 * @brief   アームの初期位置に移動させる
 *
 * @return  ev3_motor_rotate()参照
*/
ER initialize_arm_position()
{
    int LastArmDegrees = 0;
    int NotChangeDegreesCount = 0;
    ev3_motor_reset_counts(EV3_MOTOR_ARM);

    for(;;){
        //! アームを下向き側へ１度ずつ動かす
        ER result = ev3_motor_rotate(EV3_MOTOR_ARM, -1, 1, true);

        //! 各位置を
        int armDegrees = ev3_motor_get_counts(EV3_MOTOR_ARM);

        //! 前回のときの角位置と同じかどうか
        if(armDegrees == LastArmDegrees){
            NotChangeDegreesCount++;
        }

        //! 10回以上角位置が動いていない（駆動域の限界まできたと判断）
        if(NotChangeDegreesCount > 10){
            break;
        }
    }

    //! 角度が変化しないところから一定のところに戻す
    ER result = ev3_motor_rotate(EV3_MOTOR_ARM, initializePosition, 1, true);
    if (result == E_OK) {
        writeFloatLCD(ev3_motor_get_counts(EV3_MOTOR_ARM));
        ev3_speaker_play_tone(NOTE_A4, 100);
    }

}