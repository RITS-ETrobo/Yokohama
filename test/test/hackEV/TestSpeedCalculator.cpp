/**
 * @file    TestSpeedCalculator.cpp
 * @brief   This file is test file for EV3Position class.
 */
#include "TestSpeedCalculator.h"

TEST_F(TestSpeedCalculator, getSpeed)
{
    addRecord(speedCalculator, 10, 10, 1);
    checkSpeed();
    checkDirection();
}

TEST_F(TestSpeedCalculator, removeExceededTimeItem)
{
    addRecord(speedCalculator, 10, 10, 1);
    addRecord(speedCalculator, 20, 10, -2);
    addRecord(speedCalculator, 30, 10, 3);
    addRecord(speedCalculator, 40, 10, -4);
    addRecord(speedCalculator, 50, 10, 5);
    addRecord(speedCalculator, 60, 10, -6);
    addRecord(speedCalculator, 70, 10, 7);
    addRecord(speedCalculator, 80, 10, -8);
    addRecord(speedCalculator, 90, 10, 9);
    addRecord(speedCalculator, 100, 10, -10);
    addRecord(speedCalculator, 110, 10, 11);
    checkSpeed();
    checkDirection();

    addRecord(speedCalculator, 150, 10, -12);
    checkSpeed();
    checkDirection();
}

TEST_F(TestSpeedCalculator, reset)
{
    addRecord(speedCalculator, 10, 10, 1);
    addRecord(speedCalculator, 20, 10, -2);
    addRecord(speedCalculator, 30, 10, 3);
    addRecord(speedCalculator, 40, 10, -4);
    addRecord(speedCalculator, 50, 10, 5);
    addRecord(speedCalculator, 60, 10, -6);
    addRecord(speedCalculator, 70, 10, 7);
    addRecord(speedCalculator, 80, 10, -8);
    addRecord(speedCalculator, 90, 10, 9);
    addRecord(speedCalculator, 100, 10, -10);
    addRecord(speedCalculator, 110, 10, 11);
    checkSpeed();
    checkDirection();

    speedCalculator->reset();
    addRecord(speedCalculator, 10, 10, 1);
    addRecord(speedCalculator, 20, 10, -2);
    addRecord(speedCalculator, 30, 10, 3);
    addRecord(speedCalculator, 40, 10, -4);
    addRecord(speedCalculator, 50, 10, 5);
    checkSpeed();
    checkDirection();
}

TEST_F(TestSpeedCalculator, positionCheck)
{
    addRecord(speedCalculator, 100, 10, 0);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 30);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 45);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 60);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 90);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 120);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 135);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 150);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, 180);
    checkPosition();
}

TEST_F(TestSpeedCalculator, positionCheck_Minus)
{
    addRecord(speedCalculator, 100, 10, -30);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -45);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -60);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -90);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -120);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -135);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -150);
    checkPosition();
    speedCalculator->reset();

    addRecord(speedCalculator, 100, 10, -180);
    checkPosition();
}

TEST_F(TestSpeedCalculator, positionCheck2)
{
    addRecord(speedCalculator, 10, 10, 5);
    checkPosition();
    addRecord(speedCalculator, 20, 10, 0);
    checkPosition();
    addRecord(speedCalculator, 30, 10, 3);
    checkPosition();
    addRecord(speedCalculator, 50, 20, -2);
    checkPosition();
}

TEST_F(TestSpeedCalculator, positionCheck_straight)
{
    for (int index = 0; index < 100; index++) {
        addRecord(speedCalculator, 10, 0.07, 0);
    }

    checkPosition();
}

TEST_F(TestSpeedCalculator, positionCheck_curve)
{
    for (int index = 0; index < 100; index++) {
        addRecord(speedCalculator, 10, 0.07, 0.0001);
    }

    checkPosition();
}

TEST_F(TestSpeedCalculator, setPosition)
{
    EV3_POSITION    position;
    position.x = 71;
    position.y = 250;
    speedCalculator->setPosition(&position, 0.0F, EV3Position::CORRECT_POSITION_MAP | EV3Position::CORRECT_DIRECTION);
    checkPosition();
}
