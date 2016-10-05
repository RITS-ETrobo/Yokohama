/**
 * @file    CourseInformation.cpp
  * @brief   This file has course informations.
 */
#include "CourseInformation.h"
#include <string.h>

/**
 *  @brief  コンストラクタ
*/
CourseInformation::CourseInformation()
    : initialized(false)
{
}

/**
 *  @brief  初期化
 *  @return なし
*/
void CourseInformation::initialize(bool isForce /*= false*/)
{
    if (isForce) {
        initialized = false;
    }    

    if (initialized) {
        return;
    }

    std::vector<OBJECT_INFORMATION>().swap(partsInformation);
    std::vector<OBJECT_INFORMATION>().swap(zonesInformation);
}

/**
 *  @brief  パーツ情報を登録する
 *  @return なし
*/
void CourseInformation::addParts(int8_t idParts, float x, float y, float width, float length, colorid_t color /*= COLOR_BLACK*/, SHAPE_TYPE shape /*= SHAPE_TYPE_LINE_VERTICAL*/)
{
    addInformation(&partsInformation, idParts, x, y, width, length, color, shape);
}

/**
 *  @brief  ゾーン情報を登録する
 *  @return なし
*/
void CourseInformation::addZones(int8_t idZone, float x, float y, float width, float length, colorid_t color /*= COLOR_BLACK*/, SHAPE_TYPE shape /*= SHAPE_TYPE_SQUARE*/)
{
    addInformation(&zonesInformation, idZone, x, y, width, length, color, shape);
}

void CourseInformation::addInformation(std::vector<OBJECT_INFORMATION> *information, int8_t id, float x, float y, float width, float length, colorid_t color, SHAPE_TYPE shape)
{
    OBJECT_INFORMATION  parts;
    parts.id = id;
    parts.x = x;
    parts.y = y;
    parts.width = width;
    parts.length = length;
    parts.shape = shape;
    parts.color_id = color;

    information->push_back(parts);
}

/**
 *  @brief  走行体のいるゾーンを返す
 *  @return ゾーンのID
*/
int8_t CourseInformation::getEV3onZone(EV3_POSITION *position, OBJECT_INFORMATION *zone, bool isREAL /*= true*/)
{
    return  getEV3onPosition(position, &zonesInformation, zone, isREAL);
}

/**
 *  @brief  走行体のいるゾーンを返す
 *  @return EV3のいる場所ののID
*/
int8_t CourseInformation::getEV3onPosition(EV3_POSITION *position, std::vector<OBJECT_INFORMATION> *information, OBJECT_INFORMATION *objectInformation, bool isREAL /*= true*/)
{
    EV3_POSITION    positionMAP;
    memcpy((void*)&positionMAP, (const void*)position, sizeof(EV3_POSITION));
    if (isREAL) {
        positionMAP.x *= 7 / (float)2;
        positionMAP.y *= 7 / (float)2;
    }

    memset((void*)information, '\0', sizeof(OBJECT_INFORMATION));
    for (int index = 0; index < (int)information->size(); index++) {
        OBJECT_INFORMATION  _objectInformation = information->at(index);
        if (position->x < _objectInformation.x) {
            continue;
        }

        if (position->x > _objectInformation.x + _objectInformation.width) {
            continue;
        }

        if (position->y < _objectInformation.y) {
            continue;
        }

        if (position->y > _objectInformation.y + _objectInformation.length) {
            continue;
        }

        memcpy((void*)information, (const void*)&_objectInformation, sizeof(OBJECT_INFORMATION));
        return  _objectInformation.id;
    }

    return  -1;
}
