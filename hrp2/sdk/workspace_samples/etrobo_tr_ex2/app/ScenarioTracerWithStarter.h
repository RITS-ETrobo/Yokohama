/******************************************************************************
 *  ScenarioTracerWithStarter.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class ScenarioTracerWithStarter
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_SCENARIOTRACERWITHSTARTER_H_
#define EV3_APP_SCENARIOTRACERWITHSTARTER_H_

#include "ScenarioTracer.h"
#include "Starter.h"

class ScenarioTracerWithStarter {
public:
    ScenarioTracerWithStarter(ScenarioTracer* scenarioTracer,
                              const Starter* starter);

    void run();

private:
    enum State {
        UNDEFINED,
        WAITING_FOR_START,
        WALKING
    };

    ScenarioTracer* mScenarioTracer;
    const Starter* mStarter;
    State mState;

    void execUndefined();
    void execWaitingForStart();
    void execWalking();
};

#endif  // EV3_APP_SCENARIOTRACERWITHSTARTER_H_
