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
    , enabled(false)
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
 * @brief   ログを出力可能な状態かを更新する
 * @param   enabled_    ログを出力するかどうか
 * @return  なし
*/
void Logger::setEnabled(bool enabled_)
{
    enabled = enabled_;
}

/**
 * @brief   ログを出力可能な状態かを取得する
 * @return  true : ログ出力可能 false : ログ出力不可能
*/
bool Logger::isEnabled()
{
    return  enabled;
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

    fpLog = fopen(LOGFILE_NAME, "a+");
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
    if (!isEnabled()) {
        return;
    }

    if (!openLog()) {
        return;
    }

    vector<USER_LOG> loggerOutput = move(loggerInfo);
    for (vector<USER_LOG>::iterator it = loggerOutput.begin(); it != loggerOutput.end(); it ++ ) {
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
