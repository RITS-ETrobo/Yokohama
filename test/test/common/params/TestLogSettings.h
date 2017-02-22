/**
 * @file    TestLogSettings.h
 * @brief   This file is test file for logging functions.
 */
#pragma once

#include "test.h"

extern "C" {
#include "logSettings.h"
}

//! Testing class for logging functions
class TestLogSettings : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        initialize_logSetting();
    };

    virtual void TearDown()
    {
    };
};
