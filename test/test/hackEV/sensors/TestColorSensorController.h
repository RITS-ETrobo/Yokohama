/**
 * @file    TestColorSensorController.h
 * @brief   This file is test file for ColorSensorController class.
 */
#pragma once

#include "test.h"
#include <iostream>

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

    //! テスト対象クラスのインスタンス
    ColorSensorController*    colorSensorController;
};
