/**
 * @file    EV3Position.cpp
 * @brief   移動距離と向きから位置を算出する
 */
#include "EV3Position.h"
#include "instances.h"
#include <math.h>

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
    bool deleteValue = removeExceededTimeItem();
    direction = record.direction;
    updateSpeed();
    if (deleteValue || (size == 0)) {
        int distance = distance_record.at(distance_record.size() - 1).distance - distance_record.at(0).distance;
        movePosition(&currentPositionREAL, distance, direction, CORRECT_POSITION_REAL);
    }
}

/**
 *  @brief  要素全体が指定時間内に収まっているかを確認し、収まっていない要素を削除する
 *
 *  deque<DISTANCE_RECORD>が指定時間(duration)内に収まっていなければ、収まるまで先頭側から要素を削除する
 *  この結果、直近の指定時間内の時間と距離を保持することになる
 *  @return true : 要素を削除した
*/
bool EV3Position::removeExceededTimeItem()
{
    bool    result = false;
    for (;;) {
        std::vector<DISTANCE_RECORD>::size_type size = distance_record.size();
        if (size <= 1) {
            break;
        }

        DISTANCE_RECORD record = distance_record.at(size - 1);
        std::vector<DISTANCE_RECORD>::iterator it = distance_record.begin();
        if (it == distance_record.end()) {
            break;
        }

        if (record.currentTime - it->currentTime <= duration) {
            break;
        }

        distance_record.erase(it);
        result = true;
        if (duration == 0) {
            break;
        }
    }

    return  result;
}

/**
 *  @param  平均速度を更新する
 *  @return なし
*/
void EV3Position::updateSpeed()
{
    averageSpeed = 0.0F;

    std::vector<DISTANCE_RECORD>::size_type size = distance_record.size();
    DISTANCE_RECORD record_first = distance_record.at(0);
    DISTANCE_RECORD record_last = distance_record.at(size - 1);

    record_speed.distance = record_last.distance - record_first.distance;
    record_speed.distanceDelta = 0;
    record_speed.direction = record_last.direction;
    record_speed.directionDelta = 0;
    record_speed.currentTime = record_last.currentTime - record_first.currentTime;
    if (record_speed.currentTime == 0) {
        return;
    }

    averageSpeed = record_speed.distance / (float)(record_speed.currentTime) * 1000;
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

    memcpy((void*)record, (const void*)&record_speed, sizeof(DISTANCE_RECORD));
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

/**
 *  @brief  走行体の現在位置を取得する
 *  @param  positionREAL    実際の座標
 *  @param  positionMAP     マップ上の座標
 *  @param  direction_      向き[単位 : 度]
 *  @return true : 取得できた false : パラメーターエラー
*/
bool EV3Position::getPosition(EV3_POSITION *positionREAL, EV3_POSITION *positionMAP, float *direction_)
{
    if ((positionREAL == NULL) || (positionMAP == NULL) || (direction_ == NULL)) {
        return  false;
    }

    memcpy((void*)positionREAL, (const void*)&currentPositionREAL, sizeof(EV3_POSITION));
    memcpy((void*)positionMAP, (const void*)&currentPositionMAP, sizeof(EV3_POSITION));
    *direction_ = direction;
    return  true;
}

/**
 *  @brief  走行体の座標を指定した位置に移動させる
 *  @param  position    座標
 *  @param  distance_   前回の測定から移動した距離[単位 : cm]
 *  @param  direction_  前回の測定から移動した角度[単位 : 度]
 *  @param  updateType  どの値を更新するか
 *  @param  beCorrected 値を補正するか
 *  @return true : 動かすことができる場合
*/
bool EV3Position::movePosition(EV3_POSITION *position, int distance_, float direction_, uint8_t updateType /*= 0*/, bool beCorrected /*= true*/)
{
    if (isValidUpdateType(updateType) == false) {
        return  false;
    }

    if (updateType & (CORRECT_POSITION_REAL | CORRECT_POSITION_MAP)) {
        if (isValidPosition(position, (updateType == CORRECT_POSITION_REAL) ? true : false, beCorrected) == false) {
            return  false;
        }

        double  degreeByRadian = direction_ * 3.141592653589793 / (float)180;
        position->x += distance_ * cos(degreeByRadian);
        position->y += distance_ * sin(degreeByRadian);
    }

    synchronizePosition(*position, (updateType == CORRECT_POSITION_REAL) ? CORRECT_POSITION_MAP : CORRECT_POSITION_REAL);
    if (logger) {
        logger->addLogFloat(LOG_TYPE_EV3_POSITION_REAL_X, currentPositionREAL.x);
        logger->addLogFloat(LOG_TYPE_EV3_POSITION_REAL_Y, currentPositionREAL.y);
        logger->addLogInt(LOG_TYPE_EV3_POSITION_MAP_X, currentPositionMAP.x);
        logger->addLogInt(LOG_TYPE_EV3_POSITION_MAP_Y, currentPositionMAP.y);
        logger->addLogFloat(LOG_TYPE_EV3_DIRECTION, direction);
    }
    return  true;
}
/**
 *  @brief  走行体の現在位置を同期する
 *  @param  position    座標
 *  @param  updateType  どの値を更新するか
 *  @return なし
*/
void EV3Position::synchronizePosition(EV3_POSITION position, uint8_t updateType /*= 0*/)
{
    if (isValidUpdateType(updateType) == false) {
        return;
    }

    if (updateType & CORRECT_POSITION_REAL) {
        memcpy((void*)&currentPositionREAL, (const void*)&position, sizeof(EV3_POSITION));
        convertPostion(&currentPositionREAL, &currentPositionMAP);
    } else if (updateType & CORRECT_POSITION_MAP) {
        memcpy((void*)&currentPositionMAP, (const void*)&position, sizeof(EV3_POSITION));
        convertPostion(&currentPositionREAL, &currentPositionMAP, false);
    }
}

/**
 *  @brief  updateTypeが適正かを確認する
 *  @param  updateType  どの値を更新するか
 *  @return 値が適正ならtrue
*/
bool EV3Position::isValidUpdateType(uint8_t updateType)
{
    return  (bool)(updateType & (CORRECT_POSITION_REAL | CORRECT_POSITION_MAP | CORRECT_DIRECTION));
}

/**
 *  @brief  位置が適正かを確認する
 *  @param  position    座標
 *  @param  isPositionREAL  実際の位置を確認するか
 *  @param  beCorrected 値を補正するか
 *  @return 値が適正ならtrue
*/
bool EV3Position::isValidPosition(EV3_POSITION *position, bool isPositionREAL /*= true*/, bool beCorrected /*= true*/)
{
    if (position == NULL) {
        return  false;
    }

    return  true;
}

/**
 *  @brief  走行体の座標を指定した位置に移動させる
 *  @param  positionREAL    現実の座標
 *  @param  positionMAP     マップの座標
 *  @param  isExchangeReal2MAP  true : 現実→マップ false : マップ→現実
 *  @return true : 変換完了した場合
*/
bool EV3Position::convertPostion(EV3_POSITION *positionREAL, EV3_POSITION *positionMAP, bool isExchangeReal2MAP /*= true*/)
{
    if (isValidPosition(positionREAL) == false) {
        return  false;
    }

    if (isValidPosition(positionMAP) == false) {
        return  false;
    }

    if (isExchangeReal2MAP) {
        positionMAP->x = positionREAL->x * 2 / (float)7;
        positionMAP->y = positionREAL->y * 2 / (float)7;
    } else {
        positionREAL->x = positionMAP->x * 7 / (float)2;
        positionREAL->y = positionMAP->y * 7 / (float)2;
    }

    return  true;
}