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

//! バッテリー情報(mA)
extern const uint_t LOG_TYPE_BATTERY_mA;

//! バッテリー情報(mV)
extern const uint_t LOG_TYPE_BATTERY_mV;

//! シナリオ情報
extern const uint_t LOG_TYPE_SCENARIO;

//! シナリオ情報(距離[単位 : cm])
extern const uint_t LOG_TYPE_SCENARIO_DISTANCE;

//! シナリオ情報(向き[単位 : 度])
extern const uint_t LOG_TYPE_SCENARIO_DIRECTION;

//! シナリオ情報(入力)
extern const uint_t LOG_TYPE_SCENARIO_POWER;

//! シナリオ情報(走行シナリオの順番)
extern const uint_t LOG_TYPE_SCENARIO_PATTERN;

//! シナリオ情報(走行シナリオ完了後にストップするか)
extern const uint_t LOG_TYPE_SCENARIO_STOP;

//! 初期化
extern const uint_t LOG_TYPE_INITIALIZE;

//! 補正した左モーターの補正値
extern const uint_t LOG_TYPE_CORRECTED_RATIO_LEFT;

//! 補正した右モーターの補正値
extern const uint_t LOG_TYPE_CORRECTED_RATIO_RIGHT;

//! 補正した左モーターの出力値
extern const uint_t LOG_TYPE_CORRECTED_POWER_LEFT;

//! 補正した右モーターの出力値
extern const uint_t LOG_TYPE_CORRECTED_POWER_RIGHT;
//@}

extern void initialize_logSetting();
extern char* getLogName(uint_t logType);
