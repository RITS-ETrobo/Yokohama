/**
 * @file    CourseInformation.h
 * @brief   This file has course informations.
 */
#pragma once

#include "ev3api.h"
#include "EV3Position.h"
#include <vector>

//! \addtogroup ログの種類(hrp2/include/t_syslog.h での定義も確認する事)
//@{
typedef enum {
    //! 直線
    SHAPE_TYPE_LINE = 0,

    //! カーブ
    SHAPE_TYPE_CURVE,

    //! 円
    SHAPE_TYPE_CIRCLE,

    SHAPE_TYPE_COUNT
} SHAPE_TYPE;
//@}

//! @struct マップに描かれている情報を格納する構造体
typedef struct {
    //! ID
    int8_t  id;

    //! 開始X座標
    float   x;

    //! 開始Y座標
    float   y;

    //! 領域の幅
    float   width;

    //! 領域の長さ
    float   length;

    //! 形状
    SHAPE_TYPE  shape;

    //! 色
    colorid_t   color_id;
} OBJECT_INFORMATION;

class CourseInformation
{
public:
    explicit CourseInformation();
    virtual void initialize(bool isForce = false);
    virtual int8_t getEV3onZone(EV3_POSITION *position, OBJECT_INFORMATION *zone, bool isREAL = true);

protected:
    virtual void addParts(int8_t idParts, float x, float y, float width, float length, colorid_t color = COLOR_BLACK, SHAPE_TYPE shape = SHAPE_TYPE_LINE);
    virtual void addZones(int8_t idZone, float x, float y, float width, float length, colorid_t color = COLOR_WHITE, SHAPE_TYPE shape = SHAPE_TYPE_LINE);
    virtual int8_t getEV3onPosition(EV3_POSITION *position, std::vector<OBJECT_INFORMATION> *information, OBJECT_INFORMATION *objectInformation, bool isREAL = true);

    static const int8_t NOT_FOUND = -1;

private:
    void addInformation(std::vector<OBJECT_INFORMATION> *information, int8_t id, float x, float y, float width, float length, colorid_t color, SHAPE_TYPE shape);

    //! 初期化したかどうか
    bool    initialized;

    //! パーツ情報
    std::vector<OBJECT_INFORMATION>  partsInformation;

    //! ゾーン情報
    std::vector<OBJECT_INFORMATION>  zonesInformation;
};
