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
    : distanceLast(0.0F)
    , distance(0.0F)
    , portMotor(portMotor_)
    , speedCalculator100ms(NULL)
    , currentPower(0)
{
}

bool MotorWheel::initialize()
{
    ev3_motor_reset_counts(portMotor);
    distance = 0.0F;
    distanceLast = 0.0F;

    if (speedCalculator100ms == NULL) {
        speedCalculator100ms = new SpeedCalculator(100, (portMotor == EV3_MOTOR_LEFT) ? TYPE_RELATED_WHEEL_LEFT : TYPE_RELATED_WHEEL_RIGHT);
        if (speedCalculator100ms == NULL) {
            return  false;
        }

        speedCalculator100ms->initialize();
    }

    return  true;
}

/**
 * @brief 	     モータにパワーを設定する
 * @param  power セットするパワー値。現在セットされているパワーと同じならセットしない。
 * @retval E_OK  正常終了
 * @retval E_ID  不正のモータポート番号
 * @retval E_OBJ モータ未接続
 */
ER MotorWheel::run(int power)
{
    //! モーターのパワーが現在と同じならAPIに設定をしなおさない（モーターAPIへ無駄な付加をかけない）
    if(power == currentPower){
        return E_OK;
    }

    //! 現在の値として保持
    currentPower = power;

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
        logger->addLogFloat((portMotor == EV3_MOTOR_LEFT) ? LOG_TYPE_DISTANCE_LEFT_TOTAL : LOG_TYPE_DISTANCE_RIGHT_TOTAL, distance, true);
    }

    //! ストップした場合は現在保持しているパワー値を0にリセット
    currentPower = 0;

    return  ev3_motor_stop(portMotor, brake);
}

void MotorWheel::updateDistance()
{
    distance = Pi * EV3_WHEEL_DIAMETER_inCorrectFact * ev3_motor_get_counts(portMotor) / (float)360;
}

float MotorWheel::getDistance()
{
    return  distance;
}

int MotorWheel::getCurrentPower()
{
    return currentPower;
}

float MotorWheel::getDistanceDelta()
{
    SYSTIM  currentTime = clock->now();
    updateDistance();
    float   distanceTotal = getDistance();
    float   distanceDelta = distanceTotal - distanceLast;
    if (distanceDelta == 0) {
        return  distanceDelta;
    }

    DISTANCE_RECORD record;
    memset(&record, '\0', sizeof(DISTANCE_RECORD));
    record.currentTime = currentTime;
    record.distanceDelta = distanceDelta;
    speedCalculator100ms->add(record);

    if (logger) {
        //ログが多くなり過ぎて、異常終了する為、コメント
        //logger->addLogFloat((portMotor == EV3_MOTOR_LEFT) ? LOG_TYPE_DISTANCE_LEFT : LOG_TYPE_DISTANCE_RIGHT, distanceDelta);

        logger->addLogFloat((portMotor == EV3_MOTOR_LEFT) ? LOG_TYPE_DISTANCE_LEFT_TOTAL : LOG_TYPE_DISTANCE_RIGHT_TOTAL, distanceTotal);
    }

    distanceLast = distanceTotal;
    return  distanceDelta;
}
