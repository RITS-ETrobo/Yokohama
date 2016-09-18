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
    : EV3Position(duration_)
    , duration(duration_)
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
void SpeedCalculator::initialize()
{
    if (initialized == false) {
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
    }

    EV3Position::initialize();
    initialized = true;
}

/**
 *  @brief  要素をvectorに追加する
 *  @param  record  追加する要素
 *  @return なし
*/
void SpeedCalculator::add(DISTANCE_RECORD record)
{
    EV3Position::add(record);
}

/**
 *  @brief  平均速度を算出する
 *  @param  record  算出元の情報
 *  @return 平均速度[単位 : cm/s]
*/
float SpeedCalculator::getSpeed(DISTANCE_RECORD *record)
{
    float   averageSpeed = EV3Position::getSpeed(record);
    if(logger != NULL){
        logger->addLogFloat(logType_speed, averageSpeed);
        // logger->addLogFloat(logType_distance, record->distance);
        // logger->addLogFloat(logType_time, record->currentTime);
    }

    return  averageSpeed;
}

/**
 *  @brief  現在の向きを取得する
 *  @return 現在の向き[単位 : 度]
*/
float SpeedCalculator::getDirection()
{
    return  EV3Position::getDirection();
}
