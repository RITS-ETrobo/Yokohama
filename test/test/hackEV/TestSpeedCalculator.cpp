/**
 * @file    TestSpeedCalculator.cpp
 * @brief   This file is test file for SpeedCalculator class.
 */
#include "TestSpeedCalculator.h"

TEST_F(TestSpeedCalculator, getSpeed)
{
    addRecord(speedCalculator100ms, 10, 10);
    checkRecord();
}

TEST_F(TestSpeedCalculator, removeExceededTimeItem)
{
    addRecord(speedCalculator100ms, 10, 10);
    addRecord(speedCalculator100ms, 20, 10);
    addRecord(speedCalculator100ms, 30, 10);
    addRecord(speedCalculator100ms, 40, 10);
    addRecord(speedCalculator100ms, 50, 10);
    addRecord(speedCalculator100ms, 60, 10);
    addRecord(speedCalculator100ms, 70, 10);
    addRecord(speedCalculator100ms, 80, 10);
    addRecord(speedCalculator100ms, 90, 10);
    addRecord(speedCalculator100ms, 100, 10);
    addRecord(speedCalculator100ms, 110, 10);
    checkRecord();

    addRecord(speedCalculator100ms, 150, 10);
    checkRecord();
}
