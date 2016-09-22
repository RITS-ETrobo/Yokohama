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
#include "DriveController.h"
#include "ArmModule.h"
#include "ColorSensorController.h"
#include "GyroSensorController.h"
#include "SonarSensorController.h"

//! デストラクタでの問題回避
//! 詳細は、 https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping を参照する事
void *__dso_handle=0;

//! ログクラスのインスタンス
Logger *logger = NULL;

//! DriveControllerクラスのインスタンス
DriveController*    driveController = NULL;

//! Clockクラスのインスタンス
Clock   *clock = NULL;

//! GyroSensorControllerクラスのインスタンス
GyroSensorController* gyroSensorController = NULL;

//! SonarSensorControllerクラスのインスタンス
SonarSensorController   *sonarSensorController = NULL;

//! インスタンス作成のリトライ上限
const unsigned char RETRY_CREATE_INSTANCE = 3;

/**
 * @brief   超音波センサの制御
 * 超音波センサの参考資料 ： http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html
 * @return  なし
*/
void control_sonarsensor()
{
    if (sonarSensorController == NULL) {
        return;
    }

    //! 超音波モードの準備ができたら音が2回鳴る
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_C4, 100);

    sonarSensorController->executeSonar();
    while (sonarSensorController->isEnabled()) {
        //! タッチセンサーを押すと超音波で距離を測定
        if (!ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            continue;
        }

        sonarSensorController->confirm(sonarSensorController->executeSonar());
        if (ev3_button_is_pressed(ENTER_BUTTON)) {
            break;
        }
    }
}
/**
 * @brief   緊急停止
 *
 * @return  なし
*/
void stop_emergency(){
    ev3_motor_stop(EV3_MOTOR_LEFT,true);
    ev3_motor_stop(EV3_MOTOR_RIGHT,true);
    sonarSensorController->setEnabled(false);
    ev3_motor_stop(EV3_MOTOR_ARM,true);
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
    if (logger) {
        logger->setEnabled();
    }

    clearLCD();
    confirmBattery();
    writeStringLCD("    ");

    switch(button) {
    case BACK_BUTTON:
        writeStringLCD("BACK button click");
        syslog(LOG_NOTICE, "Back button clicked.");
        break;

    case LEFT_BUTTON:
        //! アーム動作モード
        initialize_arm();

        //! 検証としてアームを動かしたい場合は以下に記載すること

        break;

    case RIGHT_BUTTON:
        //! 本体の右ボタンで超音波モード
        writeStringLCD("RIGHT button click");
        syslog(LOG_NOTICE, "RIGHT button clicked.");

        //! 超音波センサー有効化
        sonarSensorController->setEnabled();

        //! 超音波制御
        control_sonarsensor();        

        //! 超音波センサー無効化
        sonarSensorController->setEnabled(false);
        break;

    case UP_BUTTON:
        //シナリオ走行モードの初期化処理
        initialize_run();

        //! シナリオ走行モード
        writeStringLCD("UP button click");

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
        writeStringLCD("ENTER button click");

        //シナリオ走行モードの初期化処理
        initialize_run();

        //! 準備ができたら音が3回鳴る
        ev3_speaker_play_tone(NOTE_E6, 300);
        tslp_tsk(300);

        //! テスト走行開始
        start_run_test();
        break;
        
    case DOWN_BUTTON:
    
        //! カラー名とRGBの表示
        viewColor();
        
        break;

    default:
        break;
    }

    if (logger) {
        logger->outputLog(true);
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

    writeStringLCD("INIT START");

    logger = new Logger();
    driveController = new DriveController();
    clock = new Clock();
    gyroSensorController = new GyroSensorController(EV3_SENSOR_GYRO);
    sonarSensorController = new SonarSensorController(EV3_SENSOR_SONAR);

    if (logger) {
        logger->initialize();
    }

    if (driveController) {
        driveController->initialize();
    }

    if (clock) {
        clock->reset();
    }

    if (logger) {
        logger->addLog(LOG_TYPE_INITIALIZE, "START");
    }

    if (gyroSensorController) {
        gyroSensorController->initialize();
    }

    if (sonarSensorController) {
        //! 超音波センサの初期化
        sonarSensorController->initialize();
    }

    //! Configure motors
    configure_motors();

    //! Configure sensors
    configure_sensors();

    //! 初期化が終わった時点で音を一回鳴らす（キー入力待ちを知らせる）
    ev3_speaker_play_tone(NOTE_C4, 300);

    //! Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);
    ev3_button_set_on_clicked(ENTER_BUTTON, button_clicked_handler, ENTER_BUTTON);
    ev3_button_set_on_clicked(LEFT_BUTTON, button_clicked_handler, LEFT_BUTTON);
    ev3_button_set_on_clicked(RIGHT_BUTTON, button_clicked_handler, RIGHT_BUTTON);
    ev3_button_set_on_clicked(UP_BUTTON, button_clicked_handler, UP_BUTTON);
    ev3_button_set_on_clicked(DOWN_BUTTON, button_clicked_handler, DOWN_BUTTON);

    writeStringLCD("INIT END");
    if (logger) {
        logger->addLog(LOG_TYPE_INITIALIZE, "END");
    }

    confirmBattery(true);

    //! キー入力待ち ここでwhile文があるとタスクが実行されなくなるためコメントアウト
    //while(1){}    
}

/**
 * @brief   ジャイロセンサーの値を更新する
 * @param   [in]    exinf   未使用
 * @return  なし
 */
void gyro_update_task(intptr_t exinf)
{
    if (gyroSensorController == NULL || !gyroSensorController->isEnabledGyroSensor())
    {
        return;
    }

    gyroSensorController->updateGyroRate();
    logger->addLogInt(LOG_TYPE_GYRO, gyroSensorController->getGyroRate());
}

/**
 * @brief   ログ監視タスク
 * 
 * @param   [in]    exinf  未使用
 * @return  なし
 */
void log_monitoring_task(intptr_t exinf)
{
    if (logger == NULL) {
        return;
    }

    logger->outputLog();
}
