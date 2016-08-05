/**
 * @file    RunningModule.cpp
 * @brief   This file has RunningModule.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "RunningModule.h"
#include "LCDController.h"
#include <stdlib.h>
#include "ArmModule.h"
#include <map>

//! \addtogroup 方向計算要素
//@{
//! 左モーターの回転数の合計
float sumLeftMotorRotate = 0.0F;

//! 右モーターの回転数の合計
float sumRightMotorRotate = 0.0F;

//! 向きの累積[度]
float directionSum = 0.0F;
//@}

/**
 * @enum runPattern
 * 走行パターン 
 * TRACE_RUN_PATTARN_STARTからTRACE_RUN_PATTARN_ENDまでpidParameterListのindexと対応する
 *
 * - 左側走行 : [走行体] [//線//]
 * - 右側走行 :          [//線//] [走行体]
 */
enum runPattern {
    //! トレースする走行パターン(開始)
    TRACE_RUN_PATTARN_START = 0,

    //! ライントレースしつつ、直進する
    TRACE_STRAIGHT = TRACE_RUN_PATTARN_START,
    
    //! ライントレースしつつ、直進する（ラインの左側を走行）
    TRACE_STRAIGHT_LEFT = TRACE_STRAIGHT,

    //! ライントレースしつつ、直進する（ラインの右側を走行）
    TRACE_STRAIGHT_RIGHT,

    //! ライントレースしつつ、カーブを走る
    TRACE_CURVE,
    
    //! ライントレースしつつ、カーブを走る（ラインの左側を走行）
    TRACE_CURVE_LEFT = TRACE_CURVE,
    
    //! ライントレースしつつ、カーブを走る（ラインの右側を走行）
    TRACE_CURVE_RIGHT,

    //! トレースする走行パターン(終了)
    TRACE_RUN_PATTARN_END = TRACE_CURVE_RIGHT,

    //! その場回転
    PINWHEEL, 
    
    //! 片側モーター回転(右回転)
    ONESIDE_PINWHEEL_RIGHT, 
    
    //! 片側モーター回転(左回転)
    ONESIDE_PINWHEEL_LEFT, 

    //! ライントレースせずに、直進走行する
    NOTRACE_STRAIGHT,
    
    //! トレースするラインを変更する（ライン右側へ変更）
    SWITCH_SIDE_RIGHT,
    
    //! トレースするライン縁を変更する（ライン左側へ変更）
    SWITCH_SIDE_LEFT
};

/**
 * PIDパラメータのリスト
 * この配列は、runPatternのTRACE_RUN_PATTARN_STARTからTRACE_RUN_PATTARN_ENDまでと対応する
 */
const PID_PARAMETER pidParameterList[] = {
    //! 直進用PIDパラメータ（左側走行）
    {0.775F, 0.0F, 0.0F},
    
    //! 曲線用PIDパラメータ（左側走行）
    {0.775F, 0.2F, 0.375F},
    
    //! 直進用PIDパラメータ（右側走行）
    {0.775F, 0.0F, 0.375F},
    
    //! 直進用PIDパラメータ（右側走行）
    {0.775F, 0.2F, 0.375F},
    
    //! 汎用PIDパラメータ（仮）
    {0.775F, 0.65F, 0.375F}
};

std::map<runPattern, PID_PARAMETER> PID_MAP;

/**
 * @brief   初期化処理
 * 
 * @return  なし
*/
void initialize_run() {
    //! 走行パターンとPID係数のマッピング
    for (int i = TRACE_RUN_PATTARN_START; i <= TRACE_RUN_PATTARN_END; i++) {
        PID_MAP[(runPattern)i] = pidParameterList[i];
    }
}

/**
 * @enum scenario_running
 * 走行シナリオ
 */
typedef struct {
    //! 出力(-100～+100)
    int power;

    //! 走行距離[cm]
    float distance;

    //! 向き。使わない場合は、-1。使う場合は、-360～360
    int direction;

    //! 走行パターン
    enum runPattern pattern;

    //! 走行シナリオが完了した時に急停止するか(trueの場合)
    bool stop;
} scenario_running;

//! Lコース（スタート～懸賞入口）
const scenario_running L_Start_Sweepstakes_scenario[] = {
    {30, 246.0F, -1, TRACE_STRAIGHT_RIGHT, false },
    {30, 64.5F, -1, TRACE_CURVE_RIGHT, true }
};
 
//! Lコース（懸賞入口～星取り入口）
const scenario_running L_Sweepstakes_starUP_scenario[] = {
    {30, 127.5F, -1, TRACE_STRAIGHT, false},
    {30, 63.5F, -1, TRACE_CURVE, false},
    {30, 25.0F, -1, TRACE_STRAIGHT, false},
    {30, 32.5F, -1, TRACE_CURVE, false},
    {30, 26.7F, -1, TRACE_CURVE, false},
    {30, 24.0F, -1, TRACE_CURVE, false},
    {30, 118.5F, -1, TRACE_STRAIGHT, false}
};
 
//! Lコース（星取り）
const scenario_running L_StarUP_scenario[] = {
    {30, 20.8F, -1, NOTRACE_STRAIGHT, false},
    {30, 15.2F, -1, TRACE_STRAIGHT, false},
    {30, 3.0F, -1, TRACE_STRAIGHT, false},
    {-30, 39.0F, -1, NOTRACE_STRAIGHT, false}
};
 
//! Lコース（星取り入口～ET相撲）
const scenario_running L_StarUP_Sumo_scenario[] = {
    {30, 16.4F, -1, TRACE_CURVE, false},
    {30, 21.2F, -1, TRACE_CURVE, false},
    {30, 6.5F, -1, TRACE_CURVE, false},
    {30, 46.0F, -1, TRACE_STRAIGHT, false},
    {30, 12.1F, -1, TRACE_CURVE, false},
    {30, 10.5F, -1, TRACE_CURVE, false}
};

//! Lコース（ET相撲）※この間にゲーム
const scenario_running L_Sumo_scenario[] = {
    {30, 134.2F, -1, TRACE_STRAIGHT, false}
};

//! Lコース（ET相撲後～懸賞運び入口）
const scenario_running L_Sumo_kensho_scenario[] = {
    {30, 26.5F, -1, TRACE_CURVE, false},
    {30, 24.9F, -1, TRACE_CURVE, false},
    {30, 17.0F, -1, TRACE_CURVE, false},
    {30, 67.0F, -1, TRACE_CURVE, false},
    {30, 127.5F, -1, TRACE_STRAIGHT, false}
};

//! Lコース（懸賞運び）
const scenario_running L_kensho_scenario[] = {
    {30, 22.0F, -1, NOTRACE_STRAIGHT, false},
    {30, 11.5F, -1, TRACE_STRAIGHT, false},
    {-30, 11.5F, -1, TRACE_STRAIGHT, false},
    {-30, 22.0F, -1, NOTRACE_STRAIGHT, true}
};

//! Lコース（懸賞運び～ゴール）
const scenario_running L_kensho_Goal_scenario[] = {
    {30, 64.5F, -1, TRACE_CURVE, false},
    {30, 250.0F, -1, TRACE_STRAIGHT, false}
};

//! Rコース(スタート～難所入口)
const scenario_running R_Start_enterGameArea_scenario[] = {
    {30, 44.8F, -1, TRACE_STRAIGHT, false},
    {30, 24.8F, -1, TRACE_CURVE, false}
};

//! Rコース(難所入口～ゲームエリア入口)
const scenario_running L_enterGameArea_GameArea_scenario[] = {
    {30, 37.6F, -1, NOTRACE_STRAIGHT, false}
};

//! Rコース(ゲームエリア出口～LAP)
const scenario_running R_exitGameArea_LAP_scenario[] = {
    {30, 30.4F, -1, TRACE_STRAIGHT, false},
    {30, 25.8F, -1, TRACE_STRAIGHT, false},
    {30, 36.6F, -1, TRACE_CURVE, false},
};

//! Rコース(LAP～GOAL)
const scenario_running R_LAP_GOAL_scenario[] = {
    {30, 81.1F, -1, TRACE_STRAIGHT, false},
    {30, 103.9F, -1, TRACE_STRAIGHT, false},
    {30, 47.7F, -1, TRACE_CURVE, false},
    {30, 48.6F, -1, TRACE_CURVE, false},
    {30, 131.5F, -1, TRACE_STRAIGHT, false},
    {30, 3.8F, -1, TRACE_CURVE, false},
    {30, 47.6F, -1, TRACE_CURVE, false},
    {30, 4.0F, -1, TRACE_CURVE, false},
    {30, 190.1F, -1, TRACE_STRAIGHT, false},
    {30, 32.8F, -1, TRACE_CURVE, false},
    {30, 297.0F, -1, TRACE_STRAIGHT, false},
};

//! 検証用シナリオ
const scenario_running run_scenario_test[] = {
    {60, 41.0F, -1, TRACE_STRAIGHT, false},
    {30, 43.0F, -1, TRACE_CURVE, false},
    {60, 40.0F, -1, TRACE_STRAIGHT, false},
    {30, 195.0F, -1, TRACE_CURVE, false},
    {60, 0.0F, 360, PINWHEEL, true}
};

//! 検証用シナリオ（ラインの縁の変更）
const scenario_running run_scenario_test_switch[] = {
    {20, 10.0F, -1, TRACE_STRAIGHT_RIGHT, false},
    {20, 0.0F, -1, SWITCH_SIDE_LEFT, false},
    {20, 15.0F, -1, SWITCH_SIDE_LEFT, true}
};

//! 検証用シナリオ(片側回転でのUターン)
const scenario_running run_scenario_test_UTern[] = {
    {40, 0.0F, 180, ONESIDE_PINWHEEL_RIGHT, false},
    {40, 100.0F, -1, NOTRACE_STRAIGHT, false},
    {40, 100.0F, 180, ONESIDE_PINWHEEL_LEFT, true}
};

//! 検証用シナリオ(右側走行)
const scenario_running run_scenario_test_right[] = {
    {20, 100.0F, -1, TRACE_STRAIGHT, true}
};

/**
 * @brief   瞬間の走行体の向きを取得する
 * 「瞬間の走行体の向き」とは、前回測定した位置から今回の移動までに変化した向きである
 * @return  瞬間の向き[度]
*/
float getDirectionDelta()
{
    //! 走行体の向き[度]
    float direction = ((motorWheelRight->getDistanceDelta() - motorWheelLeft->getDistanceDelta()) / EV3_TREAD) * 180 / Pi;
    return direction;
}

/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * 
 * @return  走行距離[cm]
*/
float getDistance(float rightDistance, float leftDistance)
{
    float distance = (rightDistance + leftDistance) / 2.0F;
    return distance;
}

/**
 * @brief   ストップ処理
 * 
 * @return  なし
*/
void stop_run()
{
    ev3_speaker_play_tone(NOTE_E6, 100);
    motorWheelLeft->stop();
    motorWheelRight->stop();
}

/**
 * @brief   その場回転
 * 
 * @return  なし
 */
void pinWheel(int power){
    ev3_motor_set_power(EV3_MOTOR_LEFT, (-power));
    ev3_motor_set_power(EV3_MOTOR_RIGHT, power);
}

/**
 * @brief   モーターの回転角、距離、方向を0に戻す
 *
 * @return  なし
*/
void initialize_wheel()
{
    //! モーターの角位置、向きの累積をリセット
    directionSum = 0.0F;
    motorWheelLeft->initialize();
    motorWheelRight->initialize();
}

/**
 * @brief   ラインの縁の変更処理
 * 
 * @enum LineSide targetSide 移動させたい縁
 * @return  なし
 */
void change_LineSide(scenario_running scenario)
{   
    //! 最初に回転するタイヤ
    motor_port_t firstMoveWheel;

    //! 次に回転するタイヤ
    motor_port_t secondMoveWheel;
    
    switch (scenario.pattern) {
        case SWITCH_SIDE_LEFT:
            //! 移動したい縁が左のときは最初に右タイヤ、次に左タイヤを旋回させて切り替え
            firstMoveWheel = EV3_MOTOR_RIGHT;
            secondMoveWheel = EV3_MOTOR_LEFT;
            break;
            
        case SWITCH_SIDE_RIGHT:
            //! 移動したい縁が右のときは最初に左タイヤ、次に右タイヤを旋回させて切り替え
            firstMoveWheel = EV3_MOTOR_LEFT;
            secondMoveWheel = EV3_MOTOR_RIGHT;
            break;
            
        default:
            break;
    }
    
    //! ラインの黒線の上にいるフラグ（黒線をまたいで移動するため）
    bool onBlack = false;
    
    //! 片方だけのタイヤを反対側まで旋回
    ev3_motor_set_power(firstMoveWheel, scenario.power);
    float firstDirection = 0.0F;
    for(;;){
        //! 動いた角度を記録
        firstDirection += getDirectionDelta(); 
 
        int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);       
        if(colorValue < (black + 5)){
            onBlack = true;
        }
        
        if(colorValue > (white - 5) && onBlack){
            ev3_motor_stop(firstMoveWheel,true);
            writeFloatLCD((float)colorValue);
            break;
        }
    }
    
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    
    //! 目的の縁まで回転させたらその場回転で向きを戻す
    //ev3_motor_set_power(firstMoveWheel, (power/2));
    ev3_motor_set_power(secondMoveWheel, scenario.power);
    float secondDirection=0.0F;
    for(;;){
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        secondDirection += getDirectionDelta(); 
        
        //! 走行体が最初に動いた角度分戻ったらストップ
        if(abs(secondDirection) >= abs(firstDirection)){
            //writeFloatLCD((float)colorValue);
            stop_run();
            break;
        }
    } 
}

/**
 * @brief   シナリオに従って走る
 *
 * @param   [in] scenario 走行パラメータ
 * @return  なし
*/
void run(scenario_running scenario)
{
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    
    //! ストップ監視しつつ、走行
    for (;;) {
        //! 走行
        switch (scenario.pattern) {
	    case PINWHEEL:
            //! その場回転
            pinWheel(scenario.power);
            break;

        case NOTRACE_STRAIGHT:
            //! ライントレースせずに、直進走行する
            ev3_motor_set_power(EV3_MOTOR_LEFT, scenario.power);
            ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
            break;
            
        case SWITCH_SIDE_RIGHT:
        case SWITCH_SIDE_LEFT:
            //! ライントレースする縁を変更
            change_LineSide(scenario);
            return;
            
        case ONESIDE_PINWHEEL_RIGHT:
            //! 右回転(左タイヤのみ回転)
            ev3_motor_set_power(EV3_MOTOR_LEFT, scenario.power);
            wheelMotorRight->stop();
            break;
            
        case ONESIDE_PINWHEEL_LEFT:
            //! 左回転(右タイヤのみ回転)
            ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
            wheelMotorLeft->stop();
            break;
        
        case TRACE_STRAIGHT_RIGHT:
        case TRACE_CURVE_RIGHT:
        {
            //! ライン右側をトレース
            float pidValueForRight = (-pid_controller(PID_MAP[scenario.pattern]));
            ev3_motor_steer(EV3_MOTOR_LEFT, EV3_MOTOR_RIGHT, scenario.power, pidValueForRight);
        }
        break;

        default:
            //! ライン左側をトレース
            ev3_motor_steer(EV3_MOTOR_LEFT, EV3_MOTOR_RIGHT, scenario.power, pid_controller(PID_MAP[scenario.pattern]));
            break;
        }
        
        tslp_tsk(1);//この行の必要性については要検証

        //! 瞬間の向きを取得、累積して走行体の向きを計測
        directionSum += getDirectionDelta();   

        //! 距離判定の必要性判断
        if (scenario.distance != 0) {
            //! 走行体が指定距離走行したらストップ
            if(abs(getDistance(rightDistance, leftDistance)) >= abs(scenario.distance)){
                if(scenario.stop){
                    stop_run();
                    
                    //! 左モーターの移動距離結果をLCDに表示
                    writeFloatLCD(leftDistance);
                    
                    //! 右モーターの移動距離結果をLCDに表示
                    writeFloatLCD(rightDistance);
                }

                break;
            }
        }
        
        //! 方向判定の必要性判断
        if (scenario.direction != -1) {
            //! 走行体が指定した向きになったらストップ
            if(abs(directionSum) >= abs(scenario.direction)){
                if(scenario.stop){
                    stop_run();
                    
                    //! 走行体の向きをLCDに表示
                    writeFloatLCD(directionSum);                
                }

                break;
            }
        }
    }
}

/**
 * @brief   走る処理
 *
 * @return  なし
*/
void start_run()
{
    initialize_pid_controller();
    
    ev3_speaker_play_tone(NOTE_E6, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);
    
    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if(ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)){
            break;
        }
    }

    for (int index = 0; index < sizeof(L_Start_Sweepstakes_scenario) / sizeof(L_Start_Sweepstakes_scenario[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(L_Start_Sweepstakes_scenario[index]);
    }
}

/**
 * @brief   検証用の走行
 *
 * @return  なし
*/
void start_run_test()
{
    initialize_pid_controller();
    
    ev3_speaker_play_tone(NOTE_E6, 100);
    tslp_tsk(100);
    ev3_speaker_play_tone(NOTE_E6, 100);
    
    //! PIDの準備を終えたらタッチセンサーが押されるまで待機
    for (;;) {
        if (ev3_touch_sensor_is_pressed(EV3_SENSOR_TOUCH)) {
            break;
        }
    }
    
    for (int index = 0; index < sizeof(run_scenario_test_UTern) / sizeof(run_scenario_test_UTern[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario_test_UTern[index]);
    }
}
