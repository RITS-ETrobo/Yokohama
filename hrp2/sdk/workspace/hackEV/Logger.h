/**
 * @file    Logger.h
 * @brief   This file has Logger.
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include <vector>

#include "product.h"

#include "logSettings.h"

using namespace std;

//! ログに出力する文字列のバッファーサイズ
#define BUFFER_SIZE_LOG_MESSAGE 8

/*! @struct USER_LOG
    @brief  ログに出力する情報
*/
typedef struct {
    //! ログ情報の種別(1Bytes)
    uint8_t logType;

    //! ログ時刻(4Bytes)
    SYSTIM  logTime;

    //! ログに出力する文字列(8Bytes)
    char    log[BUFFER_SIZE_LOG_MESSAGE];
} USER_LOG;

//! Class for logging
class Logger
{
public:
    explicit Logger();
    virtual void initialize();
    virtual bool openLog();
    virtual void addLog(uint_t logType, const char* message, bool isForce = false);
    virtual void addLogFloat(uint_t logType, const float value, bool isForce = false);
    virtual void addLogInt(uint_t logType, const int value, bool isForce = false);
    virtual void addLogFloatFormatted(uint_t logType, const float value, const char *format = NULL, bool isForce = false);
    virtual void addLogIntFormatted(uint_t logType, const int value, const char *format = NULL, bool isForce = false);
    virtual void outputLog(bool doClosingLog = false);
    virtual void setEnabled(bool enabled_ = true);
    virtual bool validateAddLog(uint_t logType, SYSTIM currentTime, bool isForce);

protected:
    virtual bool isEnabled();
    virtual void closeLog();

private:
    //! ログファイルのファイルポインタ
    FILE    *fpLog;

    //! ログファイルのファイル名
    char*  LOGFILE_NAME;

    //! 蓄積するログ情報
    vector<USER_LOG> loggerInfo;

    //! ログを出力できるかどうか
    bool enabled;

    //! ヘッダーを出力したかどうか
    bool    outputHeader;
};
