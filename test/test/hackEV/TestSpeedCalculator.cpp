/**
 * @file    TestSpeedCalculator.cpp
 * @brief   This file is test file for SpeedCalculator class.
 */
#include "TestSpeedCalculator.h"

TEST_F(TestSpeedCalculator, getSpeed)
{
    addRecord(speedCalculator100ms, 10, 0.0704);

    DISTANCE_RECORD recordAverage;
    float speedAverage = speedCalculator100ms->getSpeed(&recordAverage);
    EXPECT_GT(speedAverage, 0);
    std::cout << speedAverage;
}
