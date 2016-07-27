/**
 * @file    Logger.h
 * @brief   This file has Logger.
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include <vector>

#include "logSettings.h"
#include "Clock.h"

using namespace std;
using namespace ev3api;

/*! @struct USER_LOG
    @brief  ログに出力する情報
*/
typedef struct {
    //! ログ情報の種別
	uint_t	    logtype;

    //! ログ時刻
	SYSTIM	    logtim;

    //! ログに出力する文字列
    char    log[32];
} USER_LOG;

//! Class for logging
class Logger
{
public:
    explicit Logger();
    void initialize();
    void addLog(uint_t logtype, const char* message);
    void outputLog();

private:

    //! ログファイルを収めるディレクトリ
    const char*  LOGDIRECTORY_PATH = "/ev3rt/logs";

    //! ログファイルのファイル名
    const char*  LOGFILE_NAME = "hackEV_log.csv";

    //! Clockクラスのインスタンス
    Clock*  clock;

    //! 蓄積するログ情報
    vector<USER_LOG> loggerInfo;
};
