/**
 * @file    RunningModule.cpp
 * @brief   This file has RunningModule.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "scenarioRunning.h"
#include "RunningModule.h"
#include "LCDController.h"
#include <stdlib.h>
#include "ArmModule.h"
#include <map>

//! \addtogroup 距離計算要素
//@{
//! 円周率
const float Pi = 3.14159265359F;
//@}

//! 白黒検知のバッファ
const int COLOR_BUFFER = 5;

//! カラーセンサーとタイヤの距離
const float BETWEEN_COLOR_AND_WHEEL = 3.0F;

//! \addtogroup 方向計算要素
//@{
//! 左モーターの回転数の合計
float sumLeftMotorRotate = 0.0F;

//! 右モーターの回転数の合計
float sumRightMotorRotate = 0.0F;

//! 前回の右モーターの距離[cm]
float lastRightDistance = 0.0F;

//! 前回の左モーターの距離[cm]
float lastLeftDistance = 0.0F;

//! 向きの累積[度]
float directionSum = 0.0F;
//@}

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
 * @brief   リセットしてからの指定したタイヤの走行距離を計算する
 * 
 * @param   port    計測するタイヤのモーターポート
 * @return  走行距離[cm]
*/
float distance_running(motor_port_t port)
{
    float distance = Pi * EV3_WHEEL_DIAMETER * ev3_motor_get_counts(port) / 360;
    return distance;
}

/**
 * @brief   瞬間の走行体の向きを取得する
 * 「瞬間の走行体の向き」とは、前回測定した位置から今回の移動までに変化した向きである
 * @param rightDistance 右タイヤの走行距離[cm]
 * @param leftDistance  左タイヤの走行距離[cm]
 * @return  瞬間の向き[度]
*/
float getDirectionDelta(float rightDistance, float leftDistance)
{
    float rightDistanceDelta = rightDistance - lastRightDistance;
    float leftDistanceDelta = leftDistance - lastLeftDistance;
    lastRightDistance = rightDistance;
    lastLeftDistance = leftDistance;
    
    //! 走行体の向き[度]
    float direction = ((rightDistanceDelta - leftDistanceDelta) / EV3_TREAD) * 180 / Pi;
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
    ev3_motor_stop(EV3_MOTOR_LEFT,true);
    ev3_motor_stop(EV3_MOTOR_RIGHT,true); 
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
void initialize_wheel(){
    //! モーターの角位置、向きの累積をリセット
    ev3_motor_reset_counts(EV3_MOTOR_LEFT);
    ev3_motor_reset_counts(EV3_MOTOR_RIGHT);
    lastRightDistance = 0.0F;
    lastLeftDistance = 0.0F;
    directionSum = 0.0F;
}

/**
 * @brief   ライントレースせずに直進走行
 *
 * @param   [in] 出力
 * @return  なし
*/
void noTrace_Straight_Run(int power){
    ev3_motor_set_power(EV3_MOTOR_LEFT, power);
    ev3_motor_set_power(EV3_MOTOR_RIGHT, power);
}

/**
 * @brief   走行体を位置にライントレースできる調整する ※調整後はライン右側
 * 
 * @param[in] power 回転出力
 * @param[in] foundBack 探す前の向きから後方で見つけたかどうか
 * @return  位置調整が完了したらtrue、位置調整ができなかったらfalse
 */
bool adjust_position_ToLine(int power, bool foundBack){
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    
    //! ラインまで直進
    noTrace_Straight_Run(power);
    for(;;){
        //! 走行体のタイヤの真下にラインがくるまで
        if(abs(getDistance(distance_running(EV3_MOTOR_RIGHT), distance_running(EV3_MOTOR_LEFT))) >= abs(BETWEEN_COLOR_AND_WHEEL)){
            stop_run();
            break;
        }
    }
    
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    float moveDirection = 0.0F;
    
    if(foundBack){
        //! 走行体の後方で見つけた場合は、右回転でライン縁にあわせる（※捜索開始と同じ方向を向くようにするため）
        pinWheel(-power);
    }
    else{
        //! 走行体を左回転させ、ライン縁にあわせる
        pinWheel(-power);
    }
    
    bool onBlack = false;
    for(;;){
        moveDirection += getDirectionDelta(distance_running(EV3_MOTOR_LEFT), distance_running(EV3_MOTOR_RIGHT));
        //! センサーがライン縁に当たるまで回転
        int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);
        
        //! 黒の線をまたいで縁にたどり着いたら調整完了
        if(colorValue < (black + COLOR_BUFFER)){
            onBlack = true;
        }
        if(colorValue > (midpoint - COLOR_BUFFER) && onBlack){
            stop_run();
            return true;
        }
        
        //! 回転角度の絶対値
        float absMoveDirection = abs(moveDirection);
        
        //! 360度回転しても縁が見つけられなかったら止まる
        if(absMoveDirection > 360.0F){   
            stop_run();
            return false;
        }
    }
}

/**
 * @brief   ラインを探す回転処理
 * 
 * @param[in] power 回転出力
 * @return  スタート時からラインを見つけるまでの回転角度
 */
float rotate_for_findLine(int power){
    float moveDirection = 0.0F;
    for(;;){
        //! 動いた角度を記録
        moveDirection += getDirectionDelta(distance_running(EV3_MOTOR_LEFT), distance_running(EV3_MOTOR_RIGHT));
        int colorValue = ev3_color_sensor_get_reflect(EV3_SENSOR_COLOR);
        
        //! この行がないと360度回転しても止まらず、回り続ける。最終的に落ちる。なぜ？？
        //writeFloatLCD((float)colorValue);

        if(colorValue < (black + COLOR_BUFFER)){
            //! ラインを見つけたら止まる
            stop_run();
            //return true;
            return moveDirection;
        }
        
        //! 回転角度の絶対値
        float absMoveDirection = abs(moveDirection);
        
        if(absMoveDirection > 360.0F){
            //! 360度回転しても見つけられなかったら止まる
            char message[16];
            memset(message, '\0', sizeof(message));
            sprintf(message, "notFoundLine"); 
            writeStringLCD(message);
            
            stop_run();
            //return false;
            return moveDirection;
        }
    }
}

/**
 * @brief   ラインを探す（走行体のTREDの2倍の範囲を探す)
 *  最初にその場回転して探す。それでも見つけれない場合は片側回転して探す
 * 
 * @param[in] power 回転出力
 * @return  ラインを探して位置調整に成功したらtrue、失敗したらfalse
 */
bool find_Line(int power)
{
    //! 回転角、距離、角度を初期化
    initialize_wheel();
    
    //! その場で360度回転して黒の線を見つける
    pinWheel(power);
    
    float findDirection = rotate_for_findLine(power);
    
    //! 回転角度の絶対値
    float absFindDirection = abs(findDirection);
    
    //! その場回転一周では見つけれなかった場合
    if(abs(findDirection) >= 360.0F){
        //! 片側回転で探す(回転速度はその場回転と同時間で探せるように2倍にする)
        ev3_motor_set_power(EV3_MOTOR_RIGHT, power*2);
        //findLine = rotate_for_findLine(power);
        findDirection = rotate_for_findLine(power);
    }
    
    //! 回転角度の絶対値
    absFindDirection = abs(findDirection);
    
    //! その場回転または、片側回転一周以内で発見できた場合
    if(absFindDirection <= 360.0F){
        
        //! 開始地点から見て後方で発見したかどうか
        bool foundBack = false;
        if(abs(findDirection) >= 90.0F && abs(findDirection) <= 270.0F){
            foundBack = true;
        }
        
        //! 走行体の位置をライントレースできるように調整
        return adjust_position_ToLine(power, foundBack);
    }else{
        //! その場回転、片側回転両方でもラインを見つけられなかったらfalseを返す
        return false;
    }
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
        firstDirection += getDirectionDelta(distance_running(EV3_MOTOR_LEFT), distance_running(EV3_MOTOR_RIGHT)); 
 
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
        secondDirection += getDirectionDelta(distance_running(EV3_MOTOR_LEFT), distance_running(EV3_MOTOR_RIGHT)); 
        
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
            noTrace_Straight_Run(scenario.power);
            break;
            
        case SWITCH_SIDE_RIGHT:
        case SWITCH_SIDE_LEFT:
            //! ライントレースする縁を変更
            change_LineSide(scenario);
            return;
            
        case ONESIDE_PINWHEEL_RIGHT:
            //! 右回転(左タイヤのみ回転)
             ev3_motor_set_power(EV3_MOTOR_LEFT, scenario.power);
             ev3_motor_stop(EV3_MOTOR_RIGHT,true);
            break;
            
        case ONESIDE_PINWHEEL_LEFT:
            //! 左回転(右タイヤのみ回転)
            ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
            ev3_motor_stop(EV3_MOTOR_LEFT,true);
            break;
            
        case FIND_LINE:
            //! ラインを探す(成功true、失敗false)
            find_Line(scenario.power);
            return;
        
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
        
        //! 現在の左と右のモーターの走行距離を取得
        float leftDistance = distance_running(EV3_MOTOR_LEFT);
        float rightDistance = distance_running(EV3_MOTOR_RIGHT);
        
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        directionSum += getDirectionDelta(rightDistance, leftDistance);   
        
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
    
    for (int index = 0; index < sizeof(run_scenario_test_find_Line) / sizeof(run_scenario_test_find_Line[0]); index++) {
        //! シナリオが変わるたびに音を鳴らす
        ev3_speaker_play_tone(NOTE_E4, 100);
        run(run_scenario_test_find_Line[index]);
    }
}
