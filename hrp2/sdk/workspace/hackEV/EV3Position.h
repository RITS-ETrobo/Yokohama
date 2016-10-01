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

//! @struct 位置を格納する構造体
typedef struct {
    //! X軸方向の位置[単位 : (実際の位置の場合)cm、(マップ上の位置の場合)ピクセル]
    float   x;

    //! Y軸方向の位置[単位 : (実際の位置の場合)cm、(マップ上の位置の場合)ピクセル]
    float   y;
} EV3_POSITION;

//! Class for position
class EV3Position
{
public:
    explicit EV3Position(SYSTIM duration_ = 0);
    virtual void initialize(bool isForce = false);
    virtual void reset();
    virtual void add(DISTANCE_RECORD record);
    virtual float getSpeed(DISTANCE_RECORD *record);
    virtual float getDirection();
    virtual bool getPosition(EV3_POSITION *positionREAL, EV3_POSITION *positionMAP, float *direction);
    virtual void setPosition(EV3_POSITION *position, float direction_, uint8_t updateType = 0);
    virtual void synchronizePosition(EV3_POSITION *position, uint8_t updateType = 0);
    virtual bool movePosition(EV3_POSITION *position, float distance_, float direction_, uint8_t updateType = 0, bool beCorrected = true);

    //! \addtogroup correctPosition関数とmovePosition関数のupdateTypeに指定可能なビット
    //@{
    //! 実際の位置を補正する(CORRECT_POSITION_MAPと排他)
    static const uint8_t    CORRECT_POSITION_REAL = (1 << 0);

    //! マップ上の位置を補正する(CORRECT_POSITION_REALと排他)
    static const uint8_t    CORRECT_POSITION_MAP = (1 << 1);

    //! 向きを補正する
    static const uint8_t    CORRECT_DIRECTION = (1 << 2);
    //@}

private:
    bool removeExceededTimeItem();
    void updateSpeed();
    bool convertPostion(EV3_POSITION *positionREAL, EV3_POSITION *positionMAP, bool isExchangeReal2MAP = true);
    bool isValidUpdateType(uint8_t updateType);
    bool isValidPosition(EV3_POSITION *position, bool isPositionREAL = true, bool beCorrected = true);

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

    //! \addtogroup 現在位置
    //@{
    //! 実際の位置
    EV3_POSITION    currentPositionREAL;

    //! マップ上の位置
    EV3_POSITION    currentPositionMAP;
    //@}

    //! 平均速度算出の為の情報
    DISTANCE_RECORD record_speed;
};
