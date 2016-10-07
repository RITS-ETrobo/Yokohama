/**
 * @file    EV3Position.cpp
 * @brief   移動距離と向きから位置を算出する
 */
#include "EV3Position.h"
#include "instances.h"
#include <math.h>
#include "user_function.h"

/**
 *  @brief  コンストラクタ
 *  @param  needPositionInfo_   位置情報を更新するかどうか
 *  @param  duration_   速度を求める間隔[単位 : ms]
*/
EV3Position::EV3Position(bool needPositionInfo_, SYSTIM duration_ /*= 0*/)
    : duration(duration_)
    , averageSpeed(0.0F)
    , direction(0.0F)
    , initialized(false)
    , needPositionInfo(needPositionInfo_)
{
}

/**
 *  @brief  初期化
 *  @return なし
*/
void EV3Position::initialize(bool isForce /*= false*/)
{
    if (isForce) {
        initialized = false;
    }

    if (initialized == true) {
        return;
    }

    std::vector<DISTANCE_RECORD>().swap(distance_record);
    direction = 0.0F;

    DISTANCE_RECORD record;
    memset(&record, '\0', sizeof(DISTANCE_RECORD));

#ifndef  EV3_UNITTEST
    if (clock != NULL) {
        record.currentTime = clock->now();
    }
#endif  //  EV3_UNITTEST

    add(record);

    EV3_POSITION    position;
    position.x = 0.0F;
    position.y = 0.0F;
    setPosition(&position, direction, CORRECT_POSITION_MAP | CORRECT_DIRECTION);

    initialized = true;
}

/**
*  @brief  初期化(強制的に実施)
 *  @return なし
*/
void EV3Position::reset()
{
    initialize(true);
}

/**
 *  @brief  要素をvectorに追加する
 *  @param  record  追加する要素
 *  @return なし
*/
void EV3Position::add(DISTANCE_RECORD record)
{
    record.distance = record.distanceDelta;
    record.direction = record.directionDelta;

    std::vector<DISTANCE_RECORD>::size_type size = distance_record.size();
    if (size > 0) {
        record.distance += distance_record.at(size - 1).distance;
        record.direction += distance_record.at(size - 1).direction;
    }

    distance_record.push_back(record);
    removeExceededTimeItem();
    direction = record.direction;
    updateSpeed();

    if (!needPositionInfo) {
        return;
    }

    movePosition(&currentPositionREAL, record.distanceDelta, record.direction, CORRECT_POSITION_REAL);

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

    memset((void*)&record_speed, '\0', sizeof(DISTANCE_RECORD));
    record_speed.distance = record_last.distance - record_first.distance;
    record_speed.direction = record_last.direction;
    record_speed.currentTime = record_last.currentTime - record_first.currentTime;
    if (record_speed.currentTime == 0) {
        return;
    }

    averageSpeed = record_speed.distance / (float)(record_speed.currentTime) * 1000;

#ifndef EV3_UNITTEST
    if (logger) {
        logger->addLogFloat(LOG_TYPE_AVERAGE_SPEED, averageSpeed);
    }
#endif  //  EV3_UNITTEST
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
 *  @brief  走行体の現在位置を設定する
 *  @param  position    座標
 *  @param  direction_  向き[単位 : 度]
 *  @param  updateType  更新する項目を指定する
*/
void EV3Position::setPosition(EV3_POSITION *position, float direction_, uint8_t updateType /*= 0*/)
{
    if (updateType & (CORRECT_POSITION_REAL | CORRECT_POSITION_MAP)) {
        synchronizePosition(position, updateType);
    }

    if (updateType & CORRECT_DIRECTION) {
        direction = direction_;

#ifndef EV3_UNITTEST
        if (logger) {
            logger->addLogFloat(LOG_TYPE_EV3_DIRECTION, direction);
        }
#endif  //  EV3_UNITTEST
    }
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
bool EV3Position::movePosition(EV3_POSITION *position, float distance_, float direction_, uint8_t updateType /*= 0*/, bool beCorrected /*= true*/)
{
    if (isValidUpdateType(updateType) == false) {
        return  false;
    }

    if ((distance_ != 0.0F) && (updateType & (CORRECT_POSITION_REAL | CORRECT_POSITION_MAP)) && isValidPosition(position, (bool)(updateType == CORRECT_POSITION_REAL), beCorrected)) {
        double  directionRadian = degree2radian(direction_);
        double  modValue90 = user_fmod(direction_, (float)90);
        double  modValue180 = user_fmod(direction_, (float)180);
        double  diffX = 0.0F;
        if (modValue180 != 0.0F) {
            diffX = distance_ * sin(directionRadian);
        }

        double  diffY = 0.0F;
        if (!((modValue90 == 0.0F) && (modValue180 != 0.0F))) {
            diffY = distance_ * cos(directionRadian);
        }

        position->x += (float)diffX;
        position->y += (float)diffY;

        setPosition(position, direction_, updateType);
    }

    return  true;
}
/**
 *  @brief  走行体の現在位置を同期する
 *  @param  position    座標
 *  @param  updateType  どの値を更新するか
 *  @return なし
*/
void EV3Position::synchronizePosition(EV3_POSITION *position, uint8_t updateType /*= 0*/)
{
    if (isValidUpdateType(updateType) == false) {
        return;
    }

    EV3_POSITION    positionUpdate;
    if (updateType & CORRECT_POSITION_REAL) {
        memcpy((void*)&positionUpdate, (const void*)&currentPositionREAL, sizeof(EV3_POSITION));
        if (updateType & CORRECT_POSITION_REAL_X) {
            positionUpdate.x = position->x;
        }

        if (updateType & CORRECT_POSITION_REAL_Y) {
            positionUpdate.y = position->y;
        }

        memcpy((void*)&currentPositionREAL, (const void*)&positionUpdate, sizeof(EV3_POSITION));
        convertPostion(&currentPositionREAL, &currentPositionMAP);
    } else if (updateType & CORRECT_POSITION_MAP) {
        memcpy((void*)&positionUpdate, (const void*)&currentPositionMAP, sizeof(EV3_POSITION));
        if (updateType & CORRECT_POSITION_MAP_X) {
            positionUpdate.x = position->x;
        }

        if (updateType & CORRECT_POSITION_MAP_Y) {
            positionUpdate.y = position->y;
        }

        memcpy((void*)&currentPositionMAP, (const void*)&positionUpdate, sizeof(EV3_POSITION));
        convertPostion(&currentPositionREAL, &currentPositionMAP, false);
    }

#ifndef EV3_UNITTEST
    if (logger) {
        logger->addLogFloat(LOG_TYPE_EV3_POSITION_REAL_X, currentPositionREAL.x);
        logger->addLogFloat(LOG_TYPE_EV3_POSITION_REAL_Y, currentPositionREAL.y);
        logger->addLogFloat(LOG_TYPE_EV3_POSITION_MAP_X, currentPositionMAP.x);
        logger->addLogFloat(LOG_TYPE_EV3_POSITION_MAP_Y, currentPositionMAP.y);
    }
#endif  //  EV3_UNITTEST
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
        positionMAP->x = positionREAL->x * 7 / (float)2;
        positionMAP->y = positionREAL->y * 7 / (float)2;
    } else {
        positionREAL->x = positionMAP->x * 2 / (float)7;
        positionREAL->y = positionMAP->y * 2 / (float)7;
    }

    return  true;
}
