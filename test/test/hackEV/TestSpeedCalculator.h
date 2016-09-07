/**
 * @file    TestSpeedCalculator.h
 * @brief   This file is test file for SpeedCalculator class.
 */
#pragma once

#include "gtest/gtest.h"
#include <iostream>
#include <stdint.h>

#include "SpeedCalculator.h"

//! Testing class for SpeedCalculator class
class TestSpeedCalculator : public ::testing::Test {
protected:
    virtual void SetUp() {
        speedCalculator100ms = new SpeedCalculator(100);
        speedCalculator100ms->initialize();
        addRecord(speedCalculator100ms, 0, 0);
    };

    virtual void TearDown() {
    };

    /**
     * @brief   平均速度を求める為のデータを追加する
     * @param   distance_record 平均速度を求める為のデータ
     * @param   currentTime タイマー開始からの経過時間[単位 : ms]
     * @param   distanceDelta   前回の測定から進んだ距離[単位 : cm]
     * @return  なし
    */
    void addRecord(SpeedCalculator *speedCalculator, SYSTIM currentTime, float distanceDelta)
    {
        DISTANCE_RECORD record;
        record.currentTime = currentTime;
        record.distanceDelta = distanceDelta;
        speedCalculator->add(record);
    }

    SpeedCalculator*    speedCalculator100ms;
};
