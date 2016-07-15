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
#include "RunningModule.h"
#include "Logger.h"
#include "ArmModule.h"

//! デストラクタでの問題回避
//! 詳細は、 https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping を参照する事
void *__dso_handle=0;

//! ログクラスのインスタンス
Logger *logger = NULL;

//! インスタンス作成のリトライ上限
const unsigned char RETRY_CREATE_INSTANCE = 3;

/**
 * @brief   緊急停止
 *
 * @return  なし
*/
void stop_emergency(){
    ev3_motor_stop(left_motor,true);
    ev3_motor_stop(right_motor,true);
    setEnabledSonarSensor(false);
    ev3_motor_stop(arm_motor,true);
    //! モータやセンサーを使用する場合には、再開可能な形にしておいて停止処理を追加する
}

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
        OUTPUT_LOG("BACK button click", OUTPUT_TYPE_LCD);
        syslog(LOG_NOTICE, "Back button clicked.");
        break;
        
    case LEFT_BUTTON:
        //! アーム動作モード
        initialize_arm();
        
        //! アームを上げる
        move_arm(45, 3, true);
       
        tslp_tsk(400);
        
        //!アームを下げる
        move_arm(0, 3, true);
        
        break;
        
    case RIGHT_BUTTON:
        //! 本体の右ボタンで超音波モード
        OUTPUT_LOG("RIGHT button click", OUTPUT_TYPE_LCD);
        syslog(LOG_NOTICE, "RIGHT button clicked.");
        
        //! 超音波制御
        control_sonarsensor();        
        break;

    case UP_BUTTON:
        //! シナリオ走行モード
        OUTPUT_LOG("RIGHT button click", OUTPUT_TYPE_LCD);

        //! 準備ができたら音が3回鳴る
        ev3_speaker_play_tone(NOTE_C4, 100);
        tslp_tsk(200);
        ev3_speaker_play_tone(NOTE_C4, 100);
        tslp_tsk(200);
        ev3_speaker_play_tone(NOTE_C4, 100);

        //! 走行開始
        start_run();
        break;
     
    case ENTER_BUTTON:
        //! シナリオ走行モードの検証用
        ev3_speaker_play_tone(NOTE_E6, 300);
        tslp_tsk(300);

        //! テスト走行開始
        start_run_test();
        break;

    default:
        break;
    }

    if (logger) {
        logger->closeLog();
    }
}

/**
 * @brief   メインタスク
 *
 * @param   [in]    unused  未使用
 * @return  なし
*/
void main_task(intptr_t unused) {
    setFontSize(EV3_FONT_MEDIUM);

    //! ログは重要機能の為、3回リトライする
    for (int retry = 0; retry < RETRY_CREATE_INSTANCE; retry++) {
        logger = new Logger();
        if (logger != NULL) {
            break;
        }
    }

    if (logger) {
        logger->initialize();
    }

    OUTPUT_LOG("Start Initializing", OUTPUT_TYPE_FILE + OUTPUT_TYPE_LCD);

    //! Configure motors
    configure_motors();

    //! Configure sensors
    configure_sensors();

    //! 超音波センサの初期化
    initialize_sonarsensor();
    
    //! 初期化が終わった時点で音を一回鳴らす（キー入力待ちを知らせる）
    ev3_speaker_play_tone(NOTE_C4, 300);
    
    //! Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);
    ev3_button_set_on_clicked(ENTER_BUTTON, button_clicked_handler, ENTER_BUTTON);
    ev3_button_set_on_clicked(LEFT_BUTTON, button_clicked_handler, LEFT_BUTTON);
    ev3_button_set_on_clicked(RIGHT_BUTTON, button_clicked_handler, RIGHT_BUTTON);
    ev3_button_set_on_clicked(UP_BUTTON, button_clicked_handler, UP_BUTTON);

    OUTPUT_LOG("End Initializing", OUTPUT_TYPE_FILE + OUTPUT_TYPE_LCD);

    char message[16];
    memset(message, '\0', sizeof(message));
    sprintf(message, "%04d mA %04d mV", ev3_battery_current_mA(), ev3_battery_voltage_mV()); 
    OUTPUT_LOG(message, OUTPUT_TYPE_FILE + OUTPUT_TYPE_LCD);

    //! キー入力待ち ここでwhile文があるとタスクが実行されなくなるためコメントアウト
    //while(1){}    
}
