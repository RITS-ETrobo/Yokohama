/**
 * @file    TestUserFunction.h
 * @brief   This file is test file for user functions.
 */
#pragma once

#include "test.h"

extern "C" {
#include "user_function.h"
}

//! Testing class for user functions
class TestUserFunction : public ::testing::Test {
protected:
    virtual void SetUp()
    {
    };

    virtual void TearDown()
    {
    };
};
