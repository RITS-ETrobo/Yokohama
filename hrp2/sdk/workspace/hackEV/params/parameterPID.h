/**
 * @file    parameterPID.h
 * @brief   This file has the declarations of parameter for PID.
 */
#pragma once

typedef struct {
    //! P値
    float kP;

    //! I値
    float kI;

    //! D値
    float kD;
} PID_PARAMETER;

/**
 * PIDパラメータのリスト
 * この配列は、runPatternのTRACE_RUN_PATTARN_STARTからTRACE_RUN_PATTARN_ENDまでと対応する
 */
extern const PID_PARAMETER pidParameterList[];
