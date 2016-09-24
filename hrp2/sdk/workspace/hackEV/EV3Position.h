/**
 * @file    EV3Position.h
 * @brief   移動距離と向きから位置を算出する
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

#include <vector>

//! @struct 速度と座標を算出する為の情報を格納する構造体
typedef struct {
    //! タイマー開始からの経過時間[単位 : ms]
    SYSTIM  currentTime;

    //! 前回の測定から進んだ距離(差分)[単位 : cm]
    float   distanceDelta;

    //! 現在までに進んだ距離(累積)[単位 : cm]
    float   distance;

    //! 前回からの向きの変更(差分)[単位 : 度]
    float   directionDelta;

    //! 現在の向き(累積)[単位 : 度]
    float   direction;
} DISTANCE_RECORD;

//! Class for position
class EV3Position
{
public:
    explicit EV3Position(SYSTIM duration_ = 0);
    virtual void initialize();
    virtual void reset();
    virtual void add(DISTANCE_RECORD record);
    virtual float getSpeed(DISTANCE_RECORD *record);
    virtual float getDirection();

private:
    bool removeExceededTimeItem();
    void updateSpeed();

    //! 平均速度算出に必要な時間[単位 : ms]
    SYSTIM  duration;

    //! 現時点の平均速度[単位 : cm/s]
    float   averageSpeed;

    //! 現時点の向き[単位 : 度]
    float   direction;

    //! 平均速度算出の為に必要な情報
    std::vector<DISTANCE_RECORD>    distance_record;

    //! 初期化済み判定フラグ
    bool    initialized;

    //! 平均速度算出の為の情報
    DISTANCE_RECORD record_speed;
};
