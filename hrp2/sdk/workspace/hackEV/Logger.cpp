/**
 * @file    Logger.cpp
 * @brief   This file has Logger class.
 *          
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
 * @param   message 追加するログ
 * @param   displayLCD  trueの場合、すぐにLCDにメッセージを出力する
 * @return  true    ログ追加成功
 * @return  false   ログ追加失敗
*/
void Logger::addLog(const char* message)
{
    LOGGER_INFO info;
    info.duration = 0;
    if (clock) {
        info.duration = clock->now();
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

    for (vector<LOGGER_INFO>::iterator it = loggerInfo.begin(); it != loggerInfo.end(); it ++ ) {
        char    logLine[64];
        sprintf(logLine, "%d, %s\r\n", it->duration, it->log);
        if (fputs(logLine, fpLog) == EOF) {
            break;
        }
    }

    loggerInfo.clear();
    fclose(fpLog);
}
