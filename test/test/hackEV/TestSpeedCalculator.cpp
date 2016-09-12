/**
 * @file    TestSpeedCalculator.cpp
 * @brief   This file is test file for SpeedCalculator class.
 */
#include "TestSpeedCalculator.h"

TEST_F(TestSpeedCalculator, getSpeed)
{
    addRecord(speedCalculator100ms, 10, 10);

    DISTANCE_RECORD recordAverage;
    float speedAverage = speedCalculator100ms->getSpeed(&recordAverage);
    EXPECT_GT(speedAverage, 0);
    cout << "speedAverage = " << speedAverage << endl;
    cout << "recordAverage.currentTime = " << recordAverage.currentTime << endl;
    cout << "recordAverage.distanceDelta = " << recordAverage.distanceDelta << endl;
}
