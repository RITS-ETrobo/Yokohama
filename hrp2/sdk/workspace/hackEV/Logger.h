/**
 * @file    Logger.h
 * @brief   This file has Logger.
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include <vector>

#include "Clock.h"

using namespace std;
using namespace ev3api;

static const uint8_t OUTPUT_TYPE_NONE = 0;
static const uint8_t OUTPUT_TYPE_FILE = (1 << 0);
static const uint8_t OUTPUT_TYPE_LCD = (1 << 1);

typedef struct {
    uint32_t    duration;
    char    log[32];
} LOGGER_INFO;

//! Class for logging
class Logger
{
public:
    explicit Logger();
    void initialize();
    void addLog(const char* message);
    void outputLog();

private:

    //! ログファイルを収めるディレクトリ
    const char*  LOGDIRECTORY_PATH = "/ev3rt/logs";

    //! ログファイルのファイル名
    const char*  LOGFILE_NAME = "hackEV_log.csv";

    //! Clockクラスのインスタンス
    Clock*  clock;

    //! 蓄積するログ情報
    vector<LOGGER_INFO> loggerInfo;
};
