/******************************************************************************
 *  ScenarioTracerWithStarter.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class ScenarioTracerWithStarter
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "ScenarioTracerWithStarter.h"

/**
 * コンストラクタ
 * @param scenarioTracer  シナリオトレーサ
 * @param starter         スタータ
 */
ScenarioTracerWithStarter::ScenarioTracerWithStarter(
    ScenarioTracer* scenarioTracer,
    const Starter* starter)
    : mScenarioTracer(scenarioTracer),
      mStarter(starter),
      mState(UNDEFINED) {
}

/**
 * シナリオトレースする
 */
void ScenarioTracerWithStarter::run() {
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case WALKING:
        execWalking();
        break;
    default:
        break;
    }
}

/**
 * 未定義状態の処理
 */
void ScenarioTracerWithStarter::execUndefined() {
    mState = WAITING_FOR_START;
}

/**
 * 開始待ち状態の処理
 */
void ScenarioTracerWithStarter::execWaitingForStart() {
    if (mStarter->isPushed()) {
        mState = WALKING;
    }
}

/**
 * 走行中状態の処理
 */
void ScenarioTracerWithStarter::execWalking() {
    mScenarioTracer->run();
}
