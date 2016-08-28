/**
 * @file    DriveController.h
 * @brief   走行を制御する
 */
#pragma once

//! ターゲット依存の定義
#include "ev3api.h"
#include "target_test.h"

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"

//! Class for driving
class DriveController
{
public:
    explicit DriveController();

    virtual bool initialize();
    virtual void run(scenario_running scenario);
    virtual ER stop(bool_t brake = true);

protected:
    virtual bool stopByDistance(scenario_running scenario, float distanceDelta);
    virtual bool stopByDirection(scenario_running scenario, float directionDelta);
    virtual void getDelta(float *directionDelta, float *distanceDelta);
    virtual float getDistanceRatio(float directionDelta, float distanceDelta);
    virtual void getPower(int power, int direction, float distanceDeltaRatio, int *powerLeft, int *powerRight);
    virtual float getDistance(float distanceDelta);
    virtual float getDirection(float directionDelta);
    virtual bool runAsPattern(scenario_running scenario);

private:
    void pinWheel(int power);
    void straightRun(int power);
    void change_LineSide(scenario_running scenario);

    //! 左ホイールクラスのインスタンス
    MotorWheel  *motorWheelLeft;

    //! 右ホイールクラスのインスタンス
    MotorWheel  *motorWheelRight;

    //! 前回の向きの累積[単位 : 度]
    float   directionLast = 0.0F;

    //! 向きの累積[単位 : 度]
    float   directionTotal = 0.0F;

    //! 前回の距離の累積[単位 : cm]
    float   distanceLast = 0.0F;

    //! 距離の累積[単位 : cm]
    float   distanceTotal = 0.0F;
    
    //! 最後に左タイヤの距離を取得したときの値
    float lastGetDistanceLeft =0.0F;
    
    //! 最後に右タイヤの距離を取得したときの値
    float lastGetDistanceRight =0.0F;
    
    //! 最後に左タイヤに設定した出力値
    int lastPowerLeft = 0;
    
    //! 最後に右タイヤに設定した出力値
    int lastPowerRight = 0;
    
    //! 時間間隔測定に用いるための最後に取得した時間
    SYSTIM lastTime = 0;

    //! \addtogroup 速度測定用のインスタンス
    //@{
    //! 最近の100ms
    SpeedCalculator *speedCalculator100ms;

    //! 最近の1000ms
    SpeedCalculator *speedCalculator1000ms;
    //@}
};
