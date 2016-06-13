/**
 * @file    initialize.h
 * @brief   This file has all_tinitialize
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! ターゲット依存の定義
#include "target_test.h"


//  関数のプロトタイプ宣言
#ifndef TOPPERS_MACRO_ONLY
    extern void	initialize();
    extern void button_clicked_handler(intptr_t button);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
