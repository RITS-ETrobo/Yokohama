/**
 * @file    Logger.h
 * @brief   This file has Logger.
 */
#pragma once

#include <stdio.h>
#include "Clock.h"
#include "LCDController.h"

using namespace ev3api;

//! Class for logging
class Logger
{
public:
    explicit Logger();
    bool initialize();
    void closeLog();
    bool addLog(const char* message, bool displayLCD = true);

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
