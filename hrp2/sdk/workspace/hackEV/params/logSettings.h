/**
 * @file    logSettings.h
 * @brief   This file has the settings for logging.
 */
#pragma once

#include "t_stddef.h"
#include "t_syslog.h"

//! \addtogroup ログ出力の設定
//@{
//! ログを出力しない
extern const uint8_t OUTPUT_TYPE_NONE;

//! ログをファイルに出力する場合に指定する
extern const uint8_t OUTPUT_TYPE_FILE;

//! ログをLCDに出力する場合に指定する
extern const uint8_t OUTPUT_TYPE_LCD;
//@}

//! \addtogroup ログの種類(hrp2/include/t_syslog.h での定義も確認する事)
//@{
//! PID値に関するログ
extern const uint_t LOG_TYPE_PID;

//! 色(多色)に関するログ
extern const uint_t LOG_TYPE_COLOR;

//! 色(黒)に関するログ
extern const uint_t LOG_TYPE_COLOR_BLACK;

//! 色(白)に関するログ
extern const uint_t LOG_TYPE_COLOR_WHITE;

//! 距離(瞬間)
extern const uint_t LOG_TYPE_DISTANCE;

//! 距離(累積)
extern const uint_t LOG_TYPE_DISTANCE_TOTAL;

//! 角度(瞬間)
extern const uint_t LOG_TYPE_DIRECTION;

//! 角度(累積)
extern const uint_t LOG_TYPE_DIRECTION_TOTAL;

//! ジャイロ
extern const uint_t LOG_TYPE_GYRO;

//! ログを書き出している時間
extern const uint_t LOG_TYPE_WRITE_PROCESSING;

//! 距離(右瞬間)
extern const uint_t LOG_TYPE_DISTANCE_RIGHT;

//! 距離(右累積)
extern const uint_t LOG_TYPE_DISTANCE_RIGHT_TOTAL;

//! 距離(左瞬間)
extern const uint_t LOG_TYPE_DISTANCE_LEFT;

//! 距離(左累積)
extern const uint_t LOG_TYPE_DISTANCE_LEFT_TOTAL;
//@}

extern void initialize_logSetting();
extern char* getLogName(uint_t logType);
