/**
 * @file    orientationPattern.h
 * @brief   This file has the declarations of patterns for running.
 */
#pragma once

/**
 * @enum orientationPattern
 * 方位パターン 
 * 方位の判定や指定に使用
 *
 */
enum orientationPattern {
    LEFT_PATTERN = 0,
    RIGHT_PATTERN,
    UP_PATTERN,
    DOWN_PATTERN,
    NONE_PATTERN
};