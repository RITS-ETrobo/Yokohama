/**
 * @file    Logger.h
 * @brief   This file has Logger.
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include "Clock.h"

using namespace ev3api;

static const uint8_t OUTPUT_TYPE_NONE = 0;
static const uint8_t OUTPUT_TYPE_FILE = (1 << 0);
static const uint8_t OUTPUT_TYPE_LCD = (1 << 1);

//! Class for logging
class Logger
{
public:
    explicit Logger();
    bool initialize();
    void closeLog();
    bool addLog(const char* message);

private:
    bool openLog();
    bool outputLog(const char* message);

    //! ログファイルを収めるディレクトリ
    const char*  LOGDIRECTORY_PATH = "/ev3rt/logs";

    //! ログファイルのファイル名
    const char*  LOGFILE_NAME = "hackEV_log.csv";

    //! ログファイルのファイルポインター
    FILE*   fpLog;

    //! Clockクラスのインスタンス
    Clock*  clock;
};
