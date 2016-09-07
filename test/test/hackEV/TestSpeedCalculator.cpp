/**
 * @file    TestSpeedCalculator.cpp
 * @brief   This file is test file for SpeedCalculator class.
 */
#include "TestSpeedCalculator.h"

TEST(TestSpeedCalculator, getSpeedTest)
{
    addRecord(speedCalculator, 0, 0);
    addRecord(speedCalculator, 10, 0.0704);

    DISTANCE_RECORD recordAverage;
    EXPECT_GT(float speedAverage = speedCalculator->getSpeed(&recordAverage), 0);
    std::cout << speedAverage;
}
