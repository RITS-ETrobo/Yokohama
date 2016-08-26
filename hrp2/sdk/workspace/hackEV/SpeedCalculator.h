/**
 * @file    SpeedCalculator.h
 * @brief   速度を求める
 */
#pragma once

//! ターゲット依存の定義
#include "ev3api.h"
#include "target_test.h"

#include <deque>

//! @struct 速度を算出する為の情報を格納する構造体
typedef struct {
    //! タイマー開始からの経過時間[単位 : ms]
    SYSTIM  currentTime;

    //! 前回の測定から進んだ距離[単位 : cm]
    float   distanceDelta;
} DISTANCE_RECORD;

//! Class for speed
class SpeedCalculator
{
public:
    explicit SpeedCalculator(SYSTIM duration_ = 0);

    virtual void initialize();
    virtual void add(DISTANCE_RECORD record);
    virtual float getSpeed(DISTANCE_RECORD *record);

private:
    void removeExceededTimeItem(SYSTIM currentTime);
    SYSTIM  duration;
    std::deque<DISTANCE_RECORD> distance_record;
};
