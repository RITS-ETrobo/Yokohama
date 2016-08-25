/**
 * @file    MotorWheel.cpp
 * @brief   タイヤについているモーターを制御する
 */
#include "MotorWheel.h"
#include "wheelSettings.h"
#include "utilities.h"
#include "Logger.h"
#include "instances.h"

//! Class for MotorWheel
MotorWheel::MotorWheel(motor_port_t portMotor_)
    : portMotor(portMotor_)
    , distanceDelta(0.0F)
    , distance(0.0F)
    , distanceLast(0.0F)
    , speedDelta(0.0F)
{
}

void MotorWheel::initialize()
{
    ev3_motor_reset_counts(portMotor);
    distanceDelta = 0.0F;
    distance = 0.0F;
    distanceLast = 0.0F;
    speedDelta = 0.0F;
}

/**
 * @brief 	     モータを停止する
 * @param  brake ブレーキモードの指定，@a true （ブレーキモード）, @a false （フロートモード）
 * @retval E_OK  正常終了
 * @retval E_ID  不正のモータポート番号
 * @retval E_OBJ モータ未接続
 */
ER MotorWheel::run(int power)
{
    return  ev3_motor_set_power(portMotor, power);
}

/**
 * @brief 	     モータを停止する
 * @param  brake ブレーキモードの指定，@a true （ブレーキモード）, @a false （フロートモード）
 * @retval E_OK  正常終了
 * @retval E_ID  不正のモータポート番号
 * @retval E_OBJ モータ未接続
 */
ER MotorWheel::stop(bool_t brake /*= true*/)
{
    if (logger) {
        logger->addLogFloat((portMotor == EV3_MOTOR_LEFT) ? LOG_TYPE_DISTANCE_LEFT_TOTAL : LOG_TYPE_DISTANCE_RIGHT_TOTAL, distance);
    }

    return  ev3_motor_stop(portMotor, brake);
}

void MotorWheel::updateDistance()
{
    distance = Pi * EV3_WHEEL_DIAMETER * ev3_motor_get_counts(portMotor) / (float)360;
}

float MotorWheel::getDistance()
{
    return  distance;
}

float MotorWheel::getDistanceDelta()
{
    updateDistance();
    float   distanceTotal = getDistance();
    distanceDelta = distanceTotal - distanceLast;

    if (logger && (distanceDelta > 0)) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        //logger->addLogFloat((portMotor == EV3_MOTOR_LEFT) ? LOG_TYPE_DISTANCE_LEFT : LOG_TYPE_DISTANCE_RIGHT, distanceDelta);

        logger->addLogFloat((portMotor == EV3_MOTOR_LEFT) ? LOG_TYPE_DISTANCE_LEFT_TOTAL : LOG_TYPE_DISTANCE_RIGHT_TOTAL, distance);
    }

    distanceLast = distanceTotal;
    return  distanceDelta;
}
