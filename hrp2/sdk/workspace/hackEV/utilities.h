/**
 * @file    utilities.h
 * @brief   This file has the declarations of utilitiy functions and defines.
 */

#pragma once

//! ターゲット依存の定義
#include "target_test.h"

#include "defines.h"
#include "Logger.h"
#include "LCDController.h"

//! DEBUG実行しない場合は、コメントにする事
#define DEBUG

#ifdef DEBUG
    #define _debug(x) (x)
#else
    #define _debug(x)
#endif  //  DEBUG

//! ログクラスのインスタンス
extern Logger*  logger;

#define OUTPUT_LOG(message, type) \
    if (type != OUTPUT_TYPE_NONE) { \
        if (type & OUTPUT_TYPE_LCD) { \
            writeStringLCD(message); \
        } \
        if (logger && (type & OUTPUT_TYPE_FILE)) { \
            logger->addLog(message); \
        } \
    }

//  関数のプロトタイプ宣言
extern void configure_motors();
extern void configure_sensors();
extern int calibrate_light_intensity();
extern void writeFloatLCD(float value);
