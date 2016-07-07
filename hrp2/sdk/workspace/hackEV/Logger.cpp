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
    : fpLog(NULL)
    , clock(NULL)
{
}

/**
 * @brief   初期化する
 * @return  true    初期化成功
 * @return  false   初期化失敗
*/
bool Logger::initialize()
{
    clock = new Clock();
    if (clock) {
        clock->reset();
    }

    return  openLog();
}

/**
 * @brief   ログファイルを開く
 * @return  true    ログファイル作成成功
 * @return  false   ログファイル作成失敗
*/
bool Logger::openLog()
{
    if (fpLog) {
        return  true;
    }

    return  true;
    fpLog = fopen(LOGFILE_NAME, "w+");
    if (fpLog == NULL) {
        return  false;
    }

    return  true;
}

/**
 * @brief   ログファイルを閉じる
 * @return  なし
*/
void Logger::closeLog()
{
    if (fpLog == NULL) {
        return;
    }

    fclose(fpLog);
    fpLog = NULL;
}

/**
 * @brief   ログを追加する
 * @param   message 追加するログ
 * @param   displayLCD  trueの場合、すぐにLCDにメッセージを出力する
 * @return  true    ログ追加成功
 * @return  false   ログ追加失敗
*/
bool Logger::addLog(const char* message)
{
    return  outputLog(message);
}

/**
 * @brief   ログをファイルに出力する
 * @param   message ファイルに出力するログ
 * @return  true    ログ出力成功
 * @return  false   ログ出力失敗
*/
bool Logger::outputLog(const char* message)
{
    if (message == NULL) {
        return  false;
    }

    openLog();
    if (fpLog == NULL) {
        return  false;
    }

    char    log[512];
    memset(log, '\0', sizeof(log));
    uint32_t    duration = 0;
    if (clock) {
        duration = clock->now();
    }

    sprintf(log, "%u,%s\r\n", duration, message);
    if (fputs(log, fpLog) == EOF) {
        return  false;
    }

    return  true;
}
