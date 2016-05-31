/**
 * @file    app.c
 * @brief   This file has the main task.
 *          This program will be optimized for HackEV. EV means Educator Vehicle, Robot construction.
 *          - References
 *            - EV3組み立て図 : http://robotsquare.com/wp-content/uploads/2013/10/45544_educator.pdf
 *            - HOWTO create a Line Following Robot using Mindstorms - LEGO Reviews & Videos : http://thetechnicgear.com/2014/03/howto-create-line-following-robot-using-mindstorms/
 */

#include "ev3api.h"
#include "app.h"

//! DEBUG実行しない場合は、コメントにする事
#define DEBUG

#ifdef DEBUG
    #define _debug(x) (x)
#else
    #define _debug(x)
#endif  //  DEBUG

//! \addtogroup ポートの設定
//@{
//! Touch sensor    :   Port 1
const int touch_sensor = EV3_PORT_1;

//! Color sensor    :   Port 2
const int color_sensor = EV3_PORT_2;

//! Left motor  :   Port C
const int left_motor = EV3_PORT_C;

//! Right motor :   Port B
const int right_motor = EV3_PORT_B;
//@}

//! \addtogroup PID制御で用いる定数
//@{
//! 比例項の定数
const float KP = 0.07F;

//! 積分項の定数
const float KI = 0.3F;

//! 微分項の定数
const float KD = 1.0F;
//@}

/**
 * @brief   クリックしたボタンの情報を、ログへ出力する
 *
 * 準備 : ev3_button_set_on_clicked関数で、ボタンをこのハンドラー関数に登録する
 *
 * @param   [in]    button 押したボタン
 * @return  なし
*/
static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
#if !defined(BUILD_MODULE)
        syslog(LOG_NOTICE, "Back button clicked.");
#endif
        break;
    }
}

/**
 * @brief   メインタスク
 *
 * @param   [in]    unused  未使用
 * @return  なし
*/
void main_task(intptr_t unused) {
    //! Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    //! Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);

    //! Configure sensors
    ev3_sensor_config(touch_sensor, TOUCH_SENSOR);
    ev3_sensor_config(color_sensor, COLOR_SENSOR);

    //! Calibrate for light intensity of WHITE
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_C4, 100);
    // TODO: Calibrate using maximum mode => 100
    printf("Press the touch sensor to measure light intensity of WHITE.\n");
    while(!ev3_touch_sensor_is_pressed(touch_sensor));
    while(ev3_touch_sensor_is_pressed(touch_sensor));
    int white = ev3_color_sensor_get_reflect(color_sensor);
    printf("WHITE light intensity: %d.\n", white);

    //! Calibrate for light intensity of BLACK
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_C4, 100);
    // TODO: Calibrate using maximum mode => 100
    // TODO: Calibrate using minimum mode => 0
    printf("Press the touch sensor to measure light intensity of BLACK.\n");
    while(!ev3_touch_sensor_is_pressed(touch_sensor));
    while(ev3_touch_sensor_is_pressed(touch_sensor));
    int black = ev3_color_sensor_get_reflect(color_sensor);
    printf("BLACK light intensity: %d.\n", black);

    //! PID制御
    pid_controller(white,black);
}

/**
 * @brief PID制御を行う
 *
 * 「滑らかで安定したライントレースを行う」ためにPID制御を用いる。\n
 * PIDの各制御項(比例項、積分項、微分項)を求め、各制御項を足し合わせたものを操作量として、モーターを回転させる。
 *
 * PIDの参考資料 : http://monoist.atmarkit.co.jp/mn/articles/1007/26/news083.html
 *
 * @param   [in]    white   白の光量値
 * @param    [in]    black   黒の光量値
 *
 * @par Refer
 *  - 参照する定数 KP
 *  - 参照する定数 KI
 *  - 参照する定数 KD
 *
 * @return  なし
*/
void pid_controller(int white,int black){
    float lasterror = 0.0F, integral = 0.0F;
    float midpoint = (white - black) / 2 + black;
    while (1) {
        //! カラーセンサーによって取得された値を基に、偏差を算出する
        float error = midpoint - ev3_color_sensor_get_reflect(color_sensor);

        //! 偏差を積分した値を求める
        integral = error + integral * 0.5;

        float p = KP * error;//P値
        float i = KI * integral;//I値
        float d = KD * (error - lasterror);//D値
        float steer = p + i + d;//操作量
        ev3_motor_steer(left_motor, right_motor, 10, steer);
        lasterror = error;
        tslp_tsk(1);
    }
}
