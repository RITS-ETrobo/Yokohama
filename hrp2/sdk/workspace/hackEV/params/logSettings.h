/**
 * @file    logSettings.h
 * @brief   This file has the settings for logging.
 */
#pragma once

#include "t_stddef.h"

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

//! 色(白黒)に関するログ
extern const uint_t LOG_TYPE_COLOR_BW;

//! 距離
extern const uint_t LOG_TYPE_DISTANCE;

//! 角度(瞬間)
extern const uint_t LOG_TYPE_DIRECTION;

//! 角度(累積)
extern const uint_t LOG_TYPE_DIRECTION_STORED;

//! ジャイロ
extern const uint_t LOG_TYPE_GYRO;
//@}
