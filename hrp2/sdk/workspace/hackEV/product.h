/**
 * @file    product.h
 * @brief   This file has the includes depend on environment.
 */
#pragma once

#ifdef  EV3_UNITTEST
    #include "test.h"
#else
    //! ターゲット依存の定義
    #include "t_stddef.h"
    #include "t_syslog.h"
    #include "ev3api.h"
    #include "target_test.h"
    #include "Clock.h"

    using namespace ev3api;
#endif  //  EV3_UNITTEST
