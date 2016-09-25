/**
 * @file    TestEV3Position.h
 * @brief   This file is test file for EV3_POSITION class.
 */
#pragma once

#include "test.h"
#include <iostream>

#include "EV3Position.h"

//! Testing class for EV3_POSITION class
class TestEV3Position : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        ev3Position = new EV3Position(100);
        ev3Position->initialize();
    };

    virtual void TearDown()
    {
    };

    /**
     * @brief   平均速度と向き、位置を求める為のデータを追加する
     * @param   distance_record 平均速度を求める為のデータ
     * @param   currentTime タイマー開始からの経過時間[単位 : ms]
     * @param   distanceDelta   前回の測定から進んだ距離[単位 : cm]
     * @param   directionDelta  前回の測定からの向き[単位 : 度]
     * @return  なし
    */
    void addRecord(EV3Position *ev3Position, SYSTIM currentTime, float distanceDelta, float directionDelta)
    {
        DISTANCE_RECORD record;
        record.currentTime = currentTime;
        record.distanceDelta = distanceDelta;
        record.directionDelta = directionDelta;
        ev3Position->add(record);
    }

    void checkSpeed()
    {
        DISTANCE_RECORD recordAverage;
        float speedAverage = ev3Position->getSpeed(&recordAverage);
        EXPECT_GT(speedAverage, 0);
        cout << "speedAverage = " << speedAverage << endl;
        cout << "\ttime = " << recordAverage.currentTime << endl;
        cout << "\tdistance = " << recordAverage.distance << endl;
    }

    void checkDirection()
    {
        float   direction = ev3Position->getDirection();
        EXPECT_NE(direction, 0);
        cout << "\tdirection = " << direction << endl;
    }

    void checkPosition()
    {
        EV3_POSITION    positionREAL;
        EV3_POSITION    positionMAP;
        float   direction = 0.0F;
        ev3Position->getPosition(&positionREAL, &positionMAP, &direction);
        cout << "positionREAL\tx = " << positionREAL.x << "\ty = " << positionREAL.y << endl;
        cout << "positionMAP\tx = " << positionMAP.x << "\ty = " << positionMAP.y << endl;
        cout << "direction = " << direction << endl;
    }

    //! テスト対象クラスのインスタンス
    EV3Position*    ev3Position;
};
