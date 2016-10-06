/**
 * @file    TestColorSensorController.h
 * @brief   This file is test file for ColorSensorController class.
 */
#pragma once

#include "test.h"

#include "ColorSensorController.h"

//! Testing class for EV3_POSITION class
class TestColorSensorController : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        colorSensorController = new ColorSensorController(EV3_PORT_2);
        colorSensorController->initialize();
    };

    virtual void TearDown()
    {
    };

    void checkColor(std::string colorName, uint16_t red, uint16_t green, uint16_t blue)
    {
        rgb_raw_t   colorRGB;
        colorRGB.r = red;
        colorRGB.g = green;
        colorRGB.b = blue;
        EXPECT_EQ(colorName, colorSensorController->getColorName(&colorRGB));
    };

    //! テスト対象クラスのインスタンス
    ColorSensorController*    colorSensorController;
};
