/**
 * @file    EV3Position.cpp
 * @brief   移動距離と向きから位置を算出する
 */
#include "EV3Position.h"
#include "instances.h"

/**
 *  @brief  コンストラクタ
 *  @param  duration_   速度を求める間隔[単位 : ms]
*/
EV3Position::EV3Position(SYSTIM duration_ /*= 0*/)
    : duration(duration_)
    , averageSpeed(0.0F)
    , direction(0.0F)
    , initialized(false)
{
}

/**
 *  @brief  初期化
 *  @return なし
*/
void EV3Position::initialize()
{
    if (initialized == true) {
        return;
    }

    distance_record.clear();
    DISTANCE_RECORD record;
    memset(&record, '\0', sizeof(DISTANCE_RECORD));

#ifndef  EV3_UNITTEST
    if (clock != NULL) {
        record.currentTime = clock->now();
    }
#endif  //  EV3_UNITTEST

    add(record);
    initialized = true;
}

/**
*  @brief  初期化(強制的に実施)
 *  @return なし
*/
void EV3Position::reset()
{
    initialized = false;
    initialize();
}

/**
 *  @brief  要素をvectorに追加する
 *  @param  record  追加する要素
 *  @return なし
*/
void EV3Position::add(DISTANCE_RECORD record)
{
    std::vector<DISTANCE_RECORD>::size_type size = distance_record.size();
    if (size == 0) {
        record.distance = record.distanceDelta;
    } else {
        record.distance = distance_record.at(size - 1).distance + record.distanceDelta;
    }

    if (size == 0) {
        record.direction = record.directionDelta;
    } else {
        record.direction = distance_record.at(size - 1).direction + record.directionDelta;
    }

    distance_record.push_back(record);
    removeExceededTimeItem();
    direction = record.direction;
}

/**
 *  @brief  要素全体が指定時間内に収まっているかを確認し、収まっていない要素を削除する
 *
 *  deque<DISTANCE_RECORD>が指定時間(duration)内に収まっていなければ、収まるまで先頭側から要素を削除する
 *  この結果、直近の指定時間内の時間と距離を保持することになる
 *  @return なし
*/
void EV3Position::removeExceededTimeItem()
{
    for (;;) {
        std::vector<DISTANCE_RECORD>::size_type size = distance_record.size();
        if (size <= 1) {
            return;
        }

        DISTANCE_RECORD record = distance_record.at(size - 1);
        std::vector<DISTANCE_RECORD>::iterator it = distance_record.begin();
        if (it == distance_record.end()) {
            return;
        }

        if (record.currentTime - it->currentTime <= duration) {
            return;
        }

        distance_record.erase(it);
        if (duration == 0) {
            return;
        }
    }
}

/**
 *  @brief  平均速度を取得する
 *  @param  record  算出元の情報
 *  @return 平均速度[単位 : cm/s]
*/
float EV3Position::getSpeed(DISTANCE_RECORD *record)
{
    if (record == NULL) {
        return  0.0F;
    }

    std::vector<DISTANCE_RECORD>::size_type size = distance_record.size();
    DISTANCE_RECORD record_first = distance_record.at(0);
    DISTANCE_RECORD record_last = distance_record.at(size - 1);
    record->currentTime = record_last.currentTime - record_first.currentTime;
    record->distance = record_last.distance - record_first.distance;
    record->distanceDelta = 0;
    if (record->currentTime == 0) {
        return  0.0F;
    }

    averageSpeed = (record->distance / (float)(record->currentTime) * 1000);
    return  averageSpeed;
}

/**
 *  @brief  現在の向きを取得する
 *  @return 現在の向き[単位 : 度]
*/
float EV3Position::getDirection()
{
    return  direction;
}
