/**
 * @file    parameterPID.cpp
 * @brief   This file has the declarations of parameter for PID.
 */
#include "parameterPID.h"

/**
 * PIDパラメータのリスト
 * この配列は、runPatternのTRACE_RUN_PATTARN_STARTからTRACE_RUN_PATTARN_ENDまでと対応する
 */
const PID_PARAMETER pidParameterList[] = {
    //! 直進用PIDパラメータ（左側走行）
    {0.775F, 0.0F, 0.0F},

    //! 曲線用PIDパラメータ（左側走行）
    {0.775F, 0.2F, 0.375F},

    //! 直進用PIDパラメータ（右側走行）
    {0.775F, 0.0F, 0.375F},

    //! 直進用PIDパラメータ（右側走行）
    {0.775F, 0.2F, 0.375F},

    //! 汎用PIDパラメータ（仮）
    {0.775F, 0.65F, 0.375F}
};
