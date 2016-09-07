/**
 * @file    TestSpeedCalculator.h
 * @brief   This file is test file for SpeedCalculator class.
 */
#pragma once

#include "gtest/gtest.h"
#include <iostream>

#include "SpeedCalculator.h"

//! Testing class for SpeedCalculator class
class TestSpeedCalculator : public ::testing::Test {
protected:
    virtual void SetUp() {
        speedCalculator = new SpeedCalculator(100);
        speedCalculator->initialize();
    };

    virtual void TearDown() {
    };

    /**
     * @brief   ���ϑ��x�����߂�ׂ̃f�[�^��ǉ�����
     * @param   distance_record ���ϑ��x�����߂�ׂ̃f�[�^
     * @param   currentTime �^�C�}�[�J�n����̌o�ߎ���[�P�� : ms]
     * @param   distanceDelta   �O��̑��肩��i�񂾋���[�P�� : cm]
     * @return  �Ȃ�
    */
    void addRecord(std::deque<DISTANCE_RECORD> *distance_record, SYSTIM currentTime, float distanceDelta)
    {
        DISTANCE_RECORD record;
        record.currentTime = currentTime;
        record.distanceDelta = distanceDelta;
        distance_record->add(record);
    }

    SpeedCalculator*    speedCalculator100ms;
};
