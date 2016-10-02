/**
 * @file    SpeedCalculator.cpp
 * @brief   速度を求める
 */
#include "SpeedCalculator.h"
#include "instances.h"

/**
 *  @brief  コンストラクタ
 *  @param  duration_   速度を求める間隔[単位 : ms]
*/
SpeedCalculator::SpeedCalculator(SYSTIM duration_ /*= 0*/, typeRelated type_ /*= TYPE_RELATED_DRIVE_CONTROLLER*/)
    : EV3Position((bool)(type_ == TYPE_RELATED_DRIVE_CONTROLLER), duration_)
    , logType_speed(LOG_TYPE_AVERAGE_SPEED)
    , logType_distance(LOG_TYPE_AVERAGE_DISTANCE)
    , logType_time(LOG_TYPE_AVERAGE_TIME)
    , typeRelatedUsing(type_)
    , initialized(false)
{
}

/**
 *  @brief  初期化
 *  @return なし
*/
void SpeedCalculator::initialize(bool isForce /*= false*/)
{
    if (isForce) {
        initialized = false;
    }

    if (initialized == true) {
        return;
    }

    switch (typeRelatedUsing) {
    case TYPE_RELATED_WHEEL_LEFT:
        logType_speed = LOG_TYPE_AVERAGE_SPEED_LEFT;
        logType_distance = LOG_TYPE_AVERAGE_DISTANCE_LEFT;
        logType_time = LOG_TYPE_AVERAGE_TIME_LEFT;
        break;

    case TYPE_RELATED_WHEEL_RIGHT:
        logType_speed = LOG_TYPE_AVERAGE_SPEED_RIGHT;
        logType_distance = LOG_TYPE_AVERAGE_DISTANCE_RIGHT;
        logType_time = LOG_TYPE_AVERAGE_TIME_RIGHT;
        break;

    case TYPE_RELATED_DRIVE_CONTROLLER:
    default:
        logType_speed = LOG_TYPE_AVERAGE_SPEED;
        logType_distance = LOG_TYPE_AVERAGE_DISTANCE;
        logType_time = LOG_TYPE_AVERAGE_TIME;
        break;
    }

    EV3Position::initialize(isForce);
    initialized = true;
}

/**
 *  @brief  初期化(強制的に実施)
 *  @return なし
*/
void SpeedCalculator::reset()
{
    initialize(true);
}

/**
 *  @brief  平均速度を算出する
 *  @param  record  算出元の情報
 *  @return 平均速度[単位 : cm/s]
*/
float SpeedCalculator::getSpeed(DISTANCE_RECORD *record)
{
    float   averageSpeed = EV3Position::getSpeed(record);

#ifndef EV3_UNITTEST
    if(logger != NULL){
        logger->addLogFloat(logType_speed, averageSpeed);
        // logger->addLogFloat(logType_distance, record->distance);
        // logger->addLogFloat(logType_time, record->currentTime);
    }
#endif  //  EV3_UNITTEST

    return  averageSpeed;
}
