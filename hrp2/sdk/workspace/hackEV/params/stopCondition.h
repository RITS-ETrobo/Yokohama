/**
 * @file    stopCondition.h
 * @brief   This file has the declarations of patterns for running.
 */
#pragma once

/**
 * @enum stopCondition
 * 停止条件パターン 
 * TRACE_RUN_PATTARN_STARTからTRACE_RUN_PATTARN_ENDまでpidParameterListのindexと対応する
 *
 * - 左側走行 : [走行体] [//線//]
 * - 右側走行 :          [//線//] [走行体]
 */
enum stopCondition {
    //! 角度を停止条件にする
    DIRECTION_STOP = 0,

    //! 距離を停止条件にする
    DISTANCE_STOP
};