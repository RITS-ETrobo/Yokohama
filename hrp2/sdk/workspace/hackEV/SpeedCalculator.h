/**
 * @file    SpeedCalculator.h
 * @brief   平均速度を求める
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include "EV3Position.h"

/**
 * @enum typeRelated    どのモジュールから呼び出したかを区別する
 */
enum typeRelated {
    //! for DriveController
    TYPE_RELATED_DRIVE_CONTROLLER = 0,

    //! for Left wheel
    TYPE_RELATED_WHEEL_LEFT,

    //! for Right wheel
    TYPE_RELATED_WHEEL_RIGHT
};

//! Class for speed
class SpeedCalculator : public EV3Position
{
public:
    explicit SpeedCalculator(SYSTIM duration_ = 0, typeRelated type_ = TYPE_RELATED_DRIVE_CONTROLLER);
    virtual void initialize();
    virtual void reset();
    virtual void add(DISTANCE_RECORD record);
    virtual float getSpeed(DISTANCE_RECORD *record);
    virtual float getDirection();

private:
    //! 平均速度算出に必要な時間[単位 : ms]
    SYSTIM  duration;

    //! 速度用ログ(LOG_TYPE_AVERAGE_SPEED***)
    uint_t logType_speed;

    //! 距離用ログ(LOG_TYPE_AVERAGE_DISTANCE***)
    uint_t logType_distance;

    //! 時刻用ログ(LOG_TYPE_AVERAGE_TIME***)
    uint_t logType_time;

    //! 対象モジュール
    typeRelated typeRelatedUsing;

    //! 初期化済み判定フラグ
    bool    initialized;
};
