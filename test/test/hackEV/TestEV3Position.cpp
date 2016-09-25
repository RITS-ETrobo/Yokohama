/**
 * @file    TestEV3Position.cpp
 * @brief   This file is test file for EV3Position class.
 */
#include "TestEV3Position.h"

TEST_F(TestEV3Position, getSpeed)
{
    addRecord(ev3Position, 10, 10, 1);
    checkSpeed();
    checkDirection();
}

TEST_F(TestEV3Position, removeExceededTimeItem)
{
    addRecord(ev3Position, 10, 10, 1);
    addRecord(ev3Position, 20, 10, -2);
    addRecord(ev3Position, 30, 10, 3);
    addRecord(ev3Position, 40, 10, -4);
    addRecord(ev3Position, 50, 10, 5);
    addRecord(ev3Position, 60, 10, -6);
    addRecord(ev3Position, 70, 10, 7);
    addRecord(ev3Position, 80, 10, -8);
    addRecord(ev3Position, 90, 10, 9);
    addRecord(ev3Position, 100, 10, -10);
    addRecord(ev3Position, 110, 10, 11);
    checkSpeed();
    checkDirection();

    addRecord(ev3Position, 150, 10, -12);
    checkSpeed();
    checkDirection();
}

TEST_F(TestEV3Position, reset)
{
    addRecord(ev3Position, 10, 10, 1);
    addRecord(ev3Position, 20, 10, -2);
    addRecord(ev3Position, 30, 10, 3);
    addRecord(ev3Position, 40, 10, -4);
    addRecord(ev3Position, 50, 10, 5);
    addRecord(ev3Position, 60, 10, -6);
    addRecord(ev3Position, 70, 10, 7);
    addRecord(ev3Position, 80, 10, -8);
    addRecord(ev3Position, 90, 10, 9);
    addRecord(ev3Position, 100, 10, -10);
    addRecord(ev3Position, 110, 10, 11);
    checkSpeed();
    checkDirection();

    ev3Position->reset();
    addRecord(ev3Position, 10, 10, 1);
    addRecord(ev3Position, 20, 10, -2);
    addRecord(ev3Position, 30, 10, 3);
    addRecord(ev3Position, 40, 10, -4);
    addRecord(ev3Position, 50, 10, 5);
    checkSpeed();
    checkDirection();
}

TEST_F(TestEV3Position, positionCheck)
{
    addRecord(ev3Position, 100, 10, 0);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 30);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 45);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 60);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 90);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 120);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 135);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 150);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, 180);
    checkPosition();
}

TEST_F(TestEV3Position, positionCheck_Minus)
{
    addRecord(ev3Position, 100, 10, -30);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -45);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -60);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -90);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -120);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -135);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -150);
    checkPosition();
    ev3Position->reset();

    addRecord(ev3Position, 100, 10, -180);
    checkPosition();
}
