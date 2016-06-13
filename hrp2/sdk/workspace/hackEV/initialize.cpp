/**
 * @file    initialize.cpp
 * @brief   This file has all initialize.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "initialize.h"


/**
 * @brief   クリックしたボタンの情報を、ログへ出力する
 *
 * 準備 : ev3_button_set_on_clicked関数で、ボタンをこのハンドラー関数に登録する
 *
 * @param   [in]    button 押したボタン
 * @return  なし
*/
void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
#if !defined(BUILD_MODULE)
        syslog(LOG_NOTICE, "Back button clicked.");
#endif
        break;
        
    case LEFT_BUTTON:
#if !defined(BUILD_MODULE)
    	syslog(LOG_NOTICE, "Left button clicked.");
#endif
        break;
        
    default:
        break;
    }
}


/**
 * @brief   初期化処理
 *
 * @return  なし
*/
void initialize() {
    //! Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);
    ev3_button_set_on_clicked(ENTER_BUTTON, button_clicked_handler, ENTER_BUTTON);
    ev3_button_set_on_clicked(LEFT_BUTTON, button_clicked_handler, LEFT_BUTTON);

    //! Configure motors
    configure_motors();

    //! Configure sensors
    configure_sensors();

    //! PID制御の初期化
    initialize_pid_controller();

}
