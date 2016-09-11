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
SpeedCalculator::SpeedCalculator(SYSTIM duration_ /*= 0*/)
    : duration(duration_)
{
}

/**
 *  @brief  初期化
 *  @return なし
*/
void SpeedCalculator::initialize()
{
    distance_record.clear();
    DISTANCE_RECORD record;

#ifdef  EV3_UNITTEST
    record.currentTime = 0;
#else
    record.currentTime = clock->now();
#endif  //  EV3_UNITTEST

    record.distanceDelta = 0;
    add(record);
}

/**
 *  @brief  要素をdequeに追加する
 *  @param  record  追加する要素
 *  @return なし
*/
void SpeedCalculator::add(DISTANCE_RECORD record)
{
    distance_record.push_back(record);
    removeExceededTimeItem();
}

/**
 *  @brief  要素全体が指定時間内に収まっているかを確認し、収まっていない要素を削除する
 *
 *  deque<DISTANCE_RECORD>が指定時間(duration)内に収まっていなければ、収まるまで先頭側から要素を削除する
 *  この結果、直近の指定時間内の時間と距離を保持することになる
 *  @return なし
*/
void SpeedCalculator::removeExceededTimeItem()
{
    for (;;) {
        std::deque<DISTANCE_RECORD>::iterator  it = distance_record.begin();
        std::deque<DISTANCE_RECORD>::iterator  it_end = distance_record.end();
        if (it == it_end) {
            return;
        }

        if (it_end->currentTime - it->currentTime < duration) {
            return;
        }

        distance_record.erase(it);
        if (duration == 0) {
            distance_record.erase(it);
            break;
        }
    }
}

/**
 *  @brief  平均速度を算出する
 *  @param  record  算出元の情報
 *  @return 平均速度[単位 : cm/s]
*/
float SpeedCalculator::getSpeed(DISTANCE_RECORD *record)
{
    if (record == NULL) {
        return  0.0F;
    }

    DISTANCE_RECORD record_front = distance_record.front();
    DISTANCE_RECORD record_back = distance_record.back();
    record->currentTime = record_back.currentTime - record_front.currentTime;
    record->distanceDelta = record_back.distanceDelta - record_front.distanceDelta;
    if (record->currentTime == 0) {
        return  0.0F;
    }

    return  (record->distanceDelta / (record->currentTime / 1000));
}
