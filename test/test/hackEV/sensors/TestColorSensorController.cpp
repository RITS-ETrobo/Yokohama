/**
 * @file    TestColorSensorController.cpp
 * @brief   This file is test file for ColorSensorController class.
 */
#include "TestColorSensorController.h"

TEST_F(TestColorSensorController, getBrightness)
{
    double  brightness = colorSensorController->getBrightness(30 / 255.0F, 30 / 255.0F, 30 / 255.0F);
    std::cout << "brightness = " << brightness << std::endl;
}

TEST_F(TestColorSensorController, getBrightness2)
{
    double  brightness = colorSensorController->getBrightness(100 / 255.0F, 90 / 255.0F, 80 / 255.0F);
    std::cout << "brightness = " << brightness << std::endl;
}

TEST_F(TestColorSensorController, getBrightness_rgbRAW)
{
    rgb_raw_t   colorRGB;
    colorRGB.r = 200;
    colorRGB.g = 100;
    colorRGB.b = 50;

    double  brightness = colorSensorController->getBrightness(&colorRGB);
    std::cout << "brightness = " << brightness << std::endl;
}
