/**
 * @file    app.cpp
 * @brief   This file has the main task.
 *          This program will be optimized for HackEV. EV means Educator Vehicle, Robot construction.
 *          - References
 *            - EV3組み立て図 : http://robotsquare.com/wp-content/uploads/2013/10/45544_educator.pdf
 *            - HOWTO create a Line Following Robot using Mindstorms - LEGO Reviews & Videos : http://thetechnicgear.com/2014/03/howto-create-line-following-robot-using-mindstorms/
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "SonarSensorController.h"


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

        syslog(LOG_NOTICE, "Back button clicked.");

        break;
        
    case LEFT_BUTTON:

        //! 本体の左ボタンで走行モード
    	syslog(LOG_NOTICE, "Left button clicked.");
        
        //! PID制御の初期化
        initialize_pid_controller();

        //! PID制御のタスク登録と開始
        act_tsk(PID_CONTROLLER_TASK);

        break;
        
    case RIGHT_BUTTON:
        
        //! 本体の右ボタンで超音波モード
        syslog(LOG_NOTICE, "RIGHT button clicked.");
        
        //! 超音波制御
        control_sonarsensor();
        
        break;
        
    default:
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
    ev3_button_set_on_clicked(ENTER_BUTTON, button_clicked_handler, ENTER_BUTTON);
    ev3_button_set_on_clicked(LEFT_BUTTON, button_clicked_handler, LEFT_BUTTON);
    ev3_button_set_on_clicked(RIGHT_BUTTON, button_clicked_handler, RIGHT_BUTTON);

    //! Configure motors
    configure_motors();

    //! Configure sensors
    configure_sensors();

    //! 超音波センサの初期化
    initialize_sonarsensor();
    
    //! 初期化が終わった時点で音を一回鳴らす（キー入力待ちを知らせる）
    ev3_speaker_play_tone(NOTE_C4, 300);
    
    //! キー入力待ち
    while(1){}    
}
