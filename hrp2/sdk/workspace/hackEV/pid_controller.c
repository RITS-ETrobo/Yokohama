/**
 * @file    pid_controller.c
 * @brief   This file has the PID contoller task.
 *          This program will be optimized for HackEV. EV means Educator Vehicle, Robot construction.
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"

//! \addtogroup PID制御で用いる定数
//@{
//! 比例項の定数
const float KP = 0.07F;

//! 積分項の定数
const float KI = 0.3F;

//! 微分項の定数
const float KD = 1.0F;
//@}

//! \addtogroup PID制御で用いる変数
//@{
//! 白の光量値
int white = 0;

//! 黒の光量値
int black = 0;

//! 前回の偏差
float lasterror = 0.0F;

//! 前回の偏差を積分した値
float integral = 0.0F;

//! 白と黒の光量値の平均値
float midpoint = 0.0F;
//@}

/**
 * @brief   PID contollerの初期化処理
 * @return  なし
*/
void initialize_pid_controller()
{
    //! Calibrate for light intensity of WHITE
    white = calibrate_light_intensity();
    printf("WHITE light intensity: %d.\n", white);

    //! Calibrate for light intensity of BLACK
    black = calibrate_light_intensity();
    printf("BLACK light intensity: %d.\n", black);

    lasterror = 0.0F;
    integral = 0.0F;
    midpoint = (white - black) / 2 + black;
}

/**
 * @brief PID制御を行う
 *
 * 「滑らかで安定したライントレースを行う」ためにPID制御を用いる。\n
 * PIDの各制御項(比例項、積分項、微分項)を求め、各制御項を足し合わせたものを操作量として、モーターを回転させる。
 *
 * PIDの参考資料 : http://monoist.atmarkit.co.jp/mn/articles/1007/26/news083.html
 *
 * @par Refer
 *  - 参照する定数 KP
 *  - 参照する定数 KI
 *  - 参照する定数 KD
 *  - 参照する変数 white
 *  - 参照する変数 black
 *  - 参照する変数 lasterror
 *  - 参照する変数 integral
 *  - 参照する変数 midpoint
 *
 * @return  なし
*/
void pid_controller()
{
    //! カラーセンサーによって取得された値を基に、偏差を算出する
    float error = midpoint - ev3_color_sensor_get_reflect(color_sensor);

    //! 偏差を積分した値を求める
    integral = error + integral * 0.5;

    //! P値を求める
    float p = KP * error;

    //! I値を求める
    float i = KI * integral;

    //! D値を求める
    float d = KD * (error - lasterror);

    //! 操作量を求める
    float steer = p + i + d;

    ev3_motor_steer(left_motor, right_motor, 10, steer);
    lasterror = error;
    tslp_tsk(1);
}

/**
 * @brief [タスク]PID制御を行う
 *
 * 「滑らかで安定したライントレースを行う」ためにPID制御を用いる。\n
 * PIDの各制御項(比例項、積分項、微分項)を求め、各制御項を足し合わせたものを操作量として、モーターを回転させる。
 *
 * PIDの参考資料 : http://monoist.atmarkit.co.jp/mn/articles/1007/26/news083.html
 * @param   [in]    unused  未使用
 * @return  なし
*/
void pid_controller_task(intptr_t unused)
{
    while (1) {
        pid_controller();
    }
}
