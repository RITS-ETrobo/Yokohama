/**
 * @file    SpeedCalculator.h
 * @brief   平均速度を求める
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include <vector>

//! @struct 速度を算出する為の情報を格納する構造体
typedef struct {
    //! タイマー開始からの経過時間[単位 : ms]
    SYSTIM  currentTime;

    //! 前回の測定から進んだ距離(差分)[単位 : cm]
    float   distanceDelta;

    //! 現在までに進んだ距離(累積)[単位 : cm]
    float   distance;
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
    void removeExceededTimeItem();
    SYSTIM  duration;
    std::vector<DISTANCE_RECORD>    distance_record;
};
