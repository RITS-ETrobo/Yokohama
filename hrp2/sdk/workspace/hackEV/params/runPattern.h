/**
 * @file    runPattern.h
 * @brief   This file has the declarations of patterns for running.
 */
#pragma once

/**
 * @enum runPattern
 * 走行パターン 
 * TRACE_RUN_PATTARN_STARTからTRACE_RUN_PATTARN_ENDまでpidParameterListのindexと対応する
 *
 * - 左側走行 : [走行体] [//線//]
 * - 右側走行 :          [//線//] [走行体]
 */
enum runPattern {
    //! トレースする走行パターン(開始)
    TRACE_RUN_PATTARN_START = 0,

    //! ライントレースしつつ、直進する
    TRACE_STRAIGHT = TRACE_RUN_PATTARN_START,

    //! ライントレースしつつ、直進する（ラインの左側を走行）
    TRACE_STRAIGHT_LEFT = TRACE_STRAIGHT,

    //! ライントレースしつつ、直進する（ラインの右側を走行）
    TRACE_STRAIGHT_RIGHT,

    //! ライントレースしつつ、カーブを走る
    TRACE_CURVE,

    //! ライントレースしつつ、カーブを走る（ラインの左側を走行）
    TRACE_CURVE_LEFT = TRACE_CURVE,

    //! ライントレースしつつ、カーブを走る（ラインの右側を走行）
    TRACE_CURVE_RIGHT,

    //! トレースする走行パターン(終了)
    TRACE_RUN_PATTARN_END = TRACE_CURVE_RIGHT,

    //! その場回転
    PINWHEEL, 

    //! 片側モーター回転(右回転)
    ONESIDE_PINWHEEL_RIGHT, 

    //! 片側モーター回転(左回転)
    ONESIDE_PINWHEEL_LEFT, 

    //! ライントレースせずに、直進走行する
    NOTRACE_STRAIGHT,
    
    //! ライントレースせずに、右方向に曲線走行
    NOTRACE_CURVE_RIGHT,

    //! ライントレースせずに、左方向に曲線走行
    NOTRACE_CURVE_LEFT,

    //! トレースするラインを変更する（ライン右側へ変更）
    SWITCH_SIDE_RIGHT,

    //! トレースするライン縁を変更する（ライン左側へ変更）
    SWITCH_SIDE_LEFT
};
