/**
 * @file    MotorWheel.cpp
 * @brief   タイヤについているモーターを制御する
 */
#include "MotorWheel.h"
#include "wheelSettings.h"
#include "utilities.h"

//! Class for MotorWheel
MotorWheel::MotorWheel(motor_port_t portMotor_)
    : portMotor(portMotor_)
    , distance(0.0F)
    , distanceLast(0.0F)
{
}

void MotorWheel::initialize()
{
    ev3_motor_reset_counts(portMotor);
    distance = 0.0F;
    distanceLast = 0.0F;
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
    writeFloatLCD(distance);
    return  ev3_motor_stop(portMotor, brake);
}

void MotorWheel::updateDistance()
{
    distance = Pi * EV3_WHEEL_DIAMETER * ev3_motor_get_counts(portMotor) / 360;
}

float MotorWheel::getDistance()
{
    return  distance;
}

float MotorWheel::getDistanceDelta()
{
    updateDistance();
    float   distance = getDistance();
    float   distanceDelta = distance - distanceLast;
    distanceLast = distance;

    return  distanceDelta;
}
