/**
 * @file    logSettings.cpp
 * @brief   This file has the settings for logging.
 */
#include "logSettings.h"

//! \addtogroup ポートの設定
//@{
//! ログを出力しない
const uint8_t OUTPUT_TYPE_NONE = 0;

//! ログをファイルに出力する場合に指定する
const uint8_t OUTPUT_TYPE_FILE = (1 << 0);

//! ログをLCDに出力する場合に指定する
const uint8_t OUTPUT_TYPE_LCD = (1 << 1);
//@}

//! \addtogroup ログの種類(hrp2/include/t_syslog.h での定義も確認する事)
//@{
//! PID値に関するログ
const uint_t LOG_TYPE_PID = 0x81;

//! 色(多色)に関するログ
const uint_t LOG_TYPE_COLOR = 0x82;

//! 色(白黒)に関するログ
const uint_t LOG_TYPE_COLOR_BW = 0x83;

//! 距離
const uint_t LOG_TYPE_DISTANCE = 0x83;

//! 角度(瞬間)
const uint_t LOG_TYPE_DIRECTION = 0x84;

//! 角度(累積)
const uint_t LOG_TYPE_DIRECTION_STORED = 0x85;

//! ジャイロ
const uint_t LOG_TYPE_GYRO = 0x86;
//@}

