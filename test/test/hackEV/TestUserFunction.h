/**
 * @file    TestUserFunction.h
 * @brief   This file is test file for EV3_POSITION class.
 */
#pragma once

#include "test.h"

extern "C"
{
#include "user_function.h"
}

//! Testing class for EV3_POSITION class
class TestUserFunction : public ::testing::Test {
protected:
    virtual void SetUp()
    {
    };

    virtual void TearDown()
    {
    };
};
