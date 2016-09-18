/**
 * @file    TestEV3Position.cpp
 * @brief   This file is test file for EV3Position class.
 */
#include "TestEV3Position.h"

TEST_F(TestEV3Position, getSpeed)
{
    addRecord(ev3Position100ms, 10, 10, 1);
    checkSpeed();
    checkDirection();
}

TEST_F(TestEV3Position, removeExceededTimeItem)
{
    addRecord(ev3Position100ms, 10, 10, 1);
    addRecord(ev3Position100ms, 20, 10, -2);
    addRecord(ev3Position100ms, 30, 10, 3);
    addRecord(ev3Position100ms, 40, 10, -4);
    addRecord(ev3Position100ms, 50, 10, 5);
    addRecord(ev3Position100ms, 60, 10, -6);
    addRecord(ev3Position100ms, 70, 10, 7);
    addRecord(ev3Position100ms, 80, 10, -8);
    addRecord(ev3Position100ms, 90, 10, 9);
    addRecord(ev3Position100ms, 100, 10, -10);
    addRecord(ev3Position100ms, 110, 10, 11);
    checkSpeed();
    checkDirection();

    addRecord(ev3Position100ms, 150, 10, -12);
    checkSpeed();
    checkDirection();
}
