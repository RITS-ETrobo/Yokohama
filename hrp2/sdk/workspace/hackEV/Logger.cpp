/**
 * @file    Logger.cpp
 * @brief   This file has Logger class.
  */
#include <utility>
#include <instances.h>
#include "Logger.h"
#include <math.h>

/**
 * @brief   Loggerクラスのコンストラクター
 * @return  なし
*/
Logger::Logger()
    : fpLog(NULL)
    , loggerInfo(NULL)
    , enabled(false)
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
 * @brief   ログを出力可能な状態かを更新する
 * @param   enabled_    ログを出力するかどうか
 * @return  なし
*/
void Logger::setEnabled(bool enabled_ /*= true*/)
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

    strncpy(info.log, message, 7);
    loggerInfo.push_back(info);
}

/**
 * @brief   ログを追加する
 * floor関数で切り捨て処理をした値を表示している。マイナスとプラスの違いを考慮すること(資料参照)
 * 参考資料： http://simd.jugem.jp/?eid=32
 * @param   logType ログの種類
 * @param   value 出力する数値
 * @return  なし
*/
void Logger::addLogFloat(uint_t logType, const float value)
{
    char message[8];
    memset(message, '\0', sizeof(message));
    sprintf(message, "%.2f", floor(value * 100) / (float)100);

    addLog(logType, message);
}

/**
 * @brief   ログを追加する
 * @param   logType ログの種類
 * @param   value   出力する数値
 * @param   format  書式指定子を含む文字列
 * @return  なし
*/
void Logger::addLogFloatFormatted(uint_t logType, const float value, const char *format /*= NULL*/)
{
    if (format == NULL || format == "") {
        addLogFloat(logType, value);
        return;
    }

    char    message[8];
    memset(message, '\0', sizeof(message));
    sprintf(message, format, value);
    addLog(logType, message);
}

/**
 * @brief   ログを追加する
 * @param   logType ログの種類
 * @param   value 出力する数値
 * @return  なし
*/
void Logger::addLogInt(uint_t logType, const int value)
{
    char message[8];
    memset(message, '\0', sizeof(message));
    sprintf(message, "%d", value);

    addLog(logType, message);
}

/**
 * @brief   ログを追加する
 * @param   logType ログの種類
 * @param   value   出力する数値
 * @param   format  書式指定子を含む文字列
 * @return  なし
*/
void Logger::addLogIntFormatted(uint_t logType, const int value, const char *format /*= NULL*/)
{
    if (format == NULL || format == "") {
        addLogInt(logType, value);
        return;
    }

    char    message[8];
    memset(message, '\0', sizeof(message));
    sprintf(message, format, value);
    addLog(logType, message);
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
void Logger::outputLog(bool doClosingLog /*= false*/)
{
    if (isEnabled()) {
        if (!openLog()) {
            return;
        }

        SYSTIM  start = 0;
        if (clock) {
            start = clock->now();
        }

        vector<USER_LOG> loggerOutput = move(loggerInfo);
        if (loggerOutput.size() == 0) {
            return;
        }

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

        if (clock) {
            addLogInt(LOG_TYPE_WRITE_PROCESSING, clock->now() - start);
        }
    }

    if (doClosingLog) {
        outputLog();
        closeLog();
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

    setEnabled(false);
}
