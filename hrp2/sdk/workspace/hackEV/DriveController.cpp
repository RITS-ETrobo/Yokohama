/**
 * @file    DriveController.cpp
 * @brief   走行を制御する
 */
#include <string.h>
#include <stdlib.h>

#include "instances.h"
#include "utilities.h"

#include "wheelSettings.h"
#include "logSettings.h"
#include "portSettings.h"

#include "pid_controller.h"

#include "DriveController.h"

//! class for driving
DriveController::DriveController()
    : motorWheelLeft(NULL)
    , motorWheelRight(NULL)
    , directionLast(0.0F)
    , directionTotal(0.0F)
    , distanceLast(0.0F)
    , distanceTotal(0.0F)
{

}

/**
 *  @brief  初期化処理
 *  @return true : 成功
 *  @return false : 失敗
*/
bool DriveController::initialize()
{
    if (motorWheelLeft == NULL) {
        motorWheelLeft = new MotorWheel(EV3_MOTOR_LEFT);
    }

    if (motorWheelRight == NULL) {
        motorWheelRight = new MotorWheel(EV3_MOTOR_RIGHT);
    }

    if (!motorWheelLeft || !motorWheelRight) {
        return  false;
    }

    motorWheelLeft->initialize();
    motorWheelRight->initialize();
    directionLast = 0.0F;
    directionTotal = 0.0F;
    directionLast = 0.0F;
    directionTotal = 0.0F;

    return  true;
}

/**
 * @brief 	     モータを停止する
 * @param  brake ブレーキモードの指定，@a true （ブレーキモード）, @a false （フロートモード）
 * @retval E_OK  正常終了
 * @retval E_ID  不正のモータポート番号
 * @retval E_OBJ モータ未接続
 */
ER DriveController::stop(bool_t brake /*= true*/)
{
    ER  resultLeft = motorWheelLeft->stop(brake);
    ER  resultRight = motorWheelRight->stop(brake);

    if (resultLeft == resultRight) {
        return  resultRight;
    }

    if (resultLeft == E_OK) {
        return  resultRight;
    }

    return  resultLeft;
}

/**
 * @brief   瞬間の走行体の向きと走行体中心の移動距離を取得する
 * 「瞬間の走行体の向き」とは、前回測定した位置から今回の移動までに変化した向きである
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  なし
*/
void DriveController::getDelta(float *directionDelta, float *distanceDelta)
{
    float   distanceDeltaLeft = motorWheelLeft->getDistanceDelta();
    float   distanceDeltaRight = motorWheelRight->getDistanceDelta();
    *directionDelta = ((distanceDeltaRight - distanceDeltaLeft) / EV3_TREAD) * 180 / Pi;
    *distanceDelta = (distanceDeltaRight + distanceDeltaLeft) / 2.0F;
}

/**
 * @brief   リセットしてからの走行体中心の移動距離を計算
 * @param   distanceDelta   瞬間の走行体中心の移動距離[単位 : cm]
 * @return  走行距離[単位 : cm]
*/
float DriveController::getDistance(float distanceDelta)
{
    distanceTotal += distanceDelta;

    if (logger) {
        //! 距離をログ出力
        char message[16];
        memset(message, '\0', sizeof(message));
        sprintf(message, "%02.04f",distanceTotal);
        logger->addLog(LOG_TYPE_DISTANCE_TOTAL, message);
    }

    return  distanceTotal;
}

/**
 * @brief   リセットしてからの走行体の向きを取得する
 * @param   directionDelta  瞬間の走行体の向き[単位 : 度]
 * @return  走行体の向き[単位 : 度]
*/
float DriveController::getDirection(float directionDelta)
{
    directionTotal += directionDelta;
    if (logger) {
        char message[16];
        memset(message, '\0', sizeof(message));
        sprintf(message, "%02.04f",directionTotal);
        logger->addLog(LOG_TYPE_DIRECTION_TOTAL, message);
    }

    return  directionTotal;
}

/**
 * @brief   走行シナリオに従って走行する
 * @param   scenario    走行シナリオ
 * @return  true : 以降の処理を継続しない
 * @return  false : 以降の処理を継続する
*/
bool DriveController::runAsPattern(scenario_running scenario)
{
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
        return  true;

    case ONESIDE_PINWHEEL_RIGHT:
        //! 右回転(左タイヤのみ回転)
        ev3_motor_set_power(EV3_MOTOR_LEFT, scenario.power);
        motorWheelRight->stop();
        break;

    case ONESIDE_PINWHEEL_LEFT:
        //! 左回転(右タイヤのみ回転)
        ev3_motor_set_power(EV3_MOTOR_RIGHT, scenario.power);
        motorWheelLeft->stop();
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

    return  false;
}

/**
 * @brief   その場回転
 * 
 * @return  なし
 */
void DriveController::pinWheel(int power)
{
    ev3_motor_set_power(EV3_MOTOR_LEFT, (-power));
    ev3_motor_set_power(EV3_MOTOR_RIGHT, power);
}

/**
 * @brief   ラインの縁の変更処理
 * 
 * @enum LineSide targetSide 移動させたい縁
 * @return  なし
 */
void DriveController::change_LineSide(scenario_running scenario)
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
    float   directionDelta = 0.0F;
    float   distanceDelta = 0.0F;
    for(;;){
        //! 動いた角度を記録
        driveController->getDelta(&directionDelta, &distanceDelta);
        firstDirection += directionDelta; 

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
    
    //! モーターの回転角、距離、方向を0に戻す
    driveController->initialize();
    
    //! 目的の縁まで回転させたらその場回転で向きを戻す
    //ev3_motor_set_power(firstMoveWheel, (power/2));
    ev3_motor_set_power(secondMoveWheel, scenario.power);
    float secondDirection=0.0F;
    for(;;){
        //! 瞬間の向きを取得、累積して走行体の向きを計測
        driveController->getDelta(&directionDelta, &distanceDelta);
        secondDirection += directionDelta; 

        //! 走行体が最初に動いた角度分戻ったらストップ
        if(abs(secondDirection) >= abs(firstDirection)){
            //writeFloatLCD((float)colorValue);
            stop();
            break;
        }
    } 
}
