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
 * @return  なし
*/
void Logger::initialize()
{
    clock = new Clock();
    if (clock) {
        clock->reset();
    }

    initialize_logSetting();
}

/**
 * @brief   ログを追加する
 * @param   logType ログの種類
 * @param   message 出力するログ
 * @return  なし
*/
void Logger::addLog(uint_t logType, const char* message)
{
    USER_LOG    info;
    info.logType = logType;
    info.logTime = 0;
    if (clock) {
        info.logTime = clock->now();
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
        sprintf(logLine, "%s, %d, %s\r\n", getLogName(it->logType), it->logTime, it->log);
        if (fputs(logLine, fpLog) == EOF) {
            break;
        }
    }

    loggerInfo.clear();
    fclose(fpLog);
}
