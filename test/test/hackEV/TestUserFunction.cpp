/**
 * @file    TestUserFunction.cpp
 * @brief   This file is test file for user functions.
 */
#include "TestUserFunction.h"

TEST_F(TestUserFunction, notChange)
{
    EXPECT_EQ(90.0F, adjustValue(90.0F, -180.0F, 180.0F));
}

TEST_F(TestUserFunction, ChangeFrom270To_90)
{
    EXPECT_EQ(-90.0F, adjustValue(270.0F, -180.0F, 180.0F));
}

TEST_F(TestUserFunction, ChangeFrom_190To170)
{
    EXPECT_EQ(170.0F, adjustValue(-190.0F, -180.0F, 180.0F));
}
