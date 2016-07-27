/**
 * @file    Logger.cpp
 * @brief   This file has Logger class.
  */
#include "Logger.h"

/**
 * @brief   Loggerクラスのコンストラクター
 * @return  なし
*/
Logger::Logger()
    : clock(NULL)
    , loggerInfo(NULL)
{
}

/**
 * @brief   初期化する
 * @return  true    初期化成功
 * @return  false   初期化失敗
*/
void Logger::initialize()
{
    clock = new Clock();
    if (clock) {
        clock->reset();
    }
}

/**
 * @brief   ログを追加する
 * @param   logType ログの種類
 * @param   message 出力するログ
 * @return  なし
*/
void Logger::addLog(uint_t logtype, const char* message)
{
    USER_LOG    info;
    info.logtype = logtype;
    info.logtim = 0;
    if (clock) {
        info.logtim = clock->now();
    }

    strcpy(info.log, message);
    loggerInfo.push_back(info);
}

/**
 * @brief   ログをファイルに出力する
 * @return  なし
*/
void Logger::outputLog()
{
    FILE    *fpLog = fopen(LOGFILE_NAME, "w+");
    if (fpLog == NULL) {
        return;
    }

    for (vector<USER_LOG>::iterator it = loggerInfo.begin(); it != loggerInfo.end(); it ++ ) {
        char    logLine[64];
        sprintf(logLine, "%d, %d, %s\r\n", it->logType, it->logtim, it->log);
        if (fputs(logLine, fpLog) == EOF) {
            break;
        }
    }

    loggerInfo.clear();
    fclose(fpLog);
}
