/**
 * @file    Logger.cpp
 * @brief   This file has Logger class.
  */
#include <utility>
#include <instances.h>
#include "Logger.h"

/**
 * @brief   Loggerクラスのコンストラクター
 * @return  なし
*/
Logger::Logger()
    : fpLog(NULL)
    , loggerInfo(NULL)
    , outputHeader(false)
{
}

/**
 * @brief   初期化する
 * @return  なし
*/
void Logger::initialize()
{
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
 * @brief   ログファイルを開く
 * @return  true : 成功
 * @return  false : 失敗
*/
bool Logger::openLog()
{
    if (fpLog) {
        return  true;
    }

    fpLog = fopen(LOGFILE_NAME, "w+");
    if (!fpLog) {
        return  false;
    }

    return  true;
}

/**
 * @brief   ログをファイルに出力する
 * @return  なし
*/
void Logger::outputLog()
{
    if (!openLog()) {
        return;
    }

    vector<USER_LOG> loggerOutput = move(loggerInfo);
    for (vector<USER_LOG>::iterator it = loggerOutput.begin(); it != loggerOutput.end(); it ++ ) {
        if (!outputHeader) {
            fputs("Duration(ms),Type,Value1,Value2,Value3\r\n", fpLog);
            outputHeader = true;
        }

        char    logLine[64];
        sprintf(logLine, "%d, %s, %s\r\n", it->logTime, getLogName(it->logType), it->log);
        if (fputs(logLine, fpLog) == EOF) {
            break;
        }
    }
}

/**
 * @brief   ログファイルにクローズする
 * @return  なし
*/
void Logger::closeLog()
{
    if (!fpLog) {
        return;
    }

    fclose(fpLog);
    fpLog = NULL;
}
