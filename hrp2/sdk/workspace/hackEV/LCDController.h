/**
 * @file    LCDController.h
 * @brief   This file has LCDController.
 */
#pragma once

#include "ev3api.h"
#include <string.h>

//! ターゲット依存の定義
#include "target_test.h"

extern ER setFontSize(lcdfont_t fontSize_);
extern void setLCDColor(lcdcolor_t lcdColor_);
extern lcdcolor_t getLCDColor();
extern unsigned char getLineheight();
extern ER writeStringLCD(const char* message);
extern ER clearLCD();
