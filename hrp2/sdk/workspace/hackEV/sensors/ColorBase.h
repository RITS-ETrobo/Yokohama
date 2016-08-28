/**
 * @file    ColorBase.h
 * @brief   カラーセンサーを使って色を取得するクラスの基底クラス
 */
#pragma once

#include "ev3api.h"
#include <map>

//! Class for ColorBase
class ColorBase
{
public:
    explicit ColorBase();
    virtual uint32_t getRGB() = 0;
    virtual rgb_raw_t* getRGBRaw() = 0;
    virtual uint8_t getID() = 0;
    virtual char* getName() = 0;
    virtual bool doCalibration(uint8_t colorID) = 0;

    virtual void initialize();
    virtual uint8_t getRGB_BW(rgb_raw_t color);
    virtual uint8_t getID_BW(rgb_raw_t color);
    virtual uint8_t getID_BW(uint8_t colorBW);
    virtual char* getName_BW(rgb_raw_t color);
    virtual char* getName_BW(uint8_t colorBW);

protected:
    virtual uint8_t getLuminance(rgb_raw_t color);
    virtual uint8_t getBWID(uint8_t colorBW);

    std::map<uint8_t, char*> COLOR_NAME_MAP;
    std::map<uint8_t, rgb_raw_t> COLOR_VALUE_MAP;
    std::map<uint8_t, uint8_t> BW_VALUE_MAP;
    const uint8_t   COLOR_GRAY = 8;
};
