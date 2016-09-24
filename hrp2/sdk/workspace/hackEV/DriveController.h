/**
 * @file    DriveController.h
 * @brief   走行を制御する
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"

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
    virtual int getCorrectedAddPower(float targetDistance, float movedDistance);
    virtual void getCorrectedPower(int power, int *powerLeft, int *powerRight);
    virtual float getDistance(float distanceDelta);
    virtual float getDirection(float directionDelta);
    virtual bool runAsPattern(scenario_running scenario);
    virtual void initializeAsPattern(scenario_running scenario);
    virtual int addAdjustValue(int targetValue, int addvalue);
    virtual void getPowerForCurvatureRadius(enum runPattern pattern, float curvatureRadius, int power, int *powerLeft, int *powerRight);
    virtual bool curveRun(enum runPattern pattern, int power, float curvatureRadius);

private:
    void pinWheel(int power, int degree);
    void straightRun(int power);
    void change_LineSide(scenario_running scenario);

    //! 左ホイールクラスのインスタンス
    MotorWheel  *motorWheelLeft;

    //! 右ホイールクラスのインスタンス
    MotorWheel  *motorWheelRight;

    //! シナリオごとの向きの累積[単位 : 度]
    float   directionScenario;

    //! スタートからの向きの累積[単位 : 度]
    float   directionTotal;

    //! シナリオごとの距離の累積[単位 : cm]
    float   distanceScenario;

    //! スタートからの距離の累積[単位 : cm]
    float   distanceTotal;

    //! 最後に左タイヤに設定した出力値
    int lastPowerLeft;
    
    //! 最後に右タイヤに設定した出力値
    int lastPowerRight;
    
    //! 時間間隔測定に用いるための最後に取得した時間
    SYSTIM lastTime;
    
    //! 出力値を補正する時間間隔
    SYSTIM DURATION;

    // 指定できるパワーの限界値
    int limitPower;

    //! 1パワーあたりの速度変化量
    float speedPerOnePower;

    //! \addtogroup 速度測定用のインスタンス
    //@{
    //! 最近の100ms
    SpeedCalculator *speedCalculator100ms;
    //@}

    //! 初期化済み判定フラグ
    bool    initialized;
};
