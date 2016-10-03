/**
 * @file    DriveController.h
 * @brief   走行を制御する
 */
#pragma once

//! ターゲット依存の定義
#include "product.h"
#include <math.h>

#include "scenarioRunning.h"

#include "MotorWheel.h"
#include "SpeedCalculator.h"
#include "coordinateScenario.h"


//! Class for driving
class DriveController
{
public:
    explicit DriveController();

    virtual bool initialize();
    virtual void run(scenario_running scenario);
    virtual ER stop(bool_t brake = true);
    virtual void manageMoveCoordinate(scenario_coordinate _coordinateScenario);
    virtual void updatePosition();
    virtual void setEnabled(bool _enabled = true);
    virtual bool isEnabled();

protected:
    virtual bool stopByDistance(scenario_running scenario);
    virtual bool stopByDirection(scenario_running scenario);
    virtual void getDelta(float *directionDelta, float *distanceDelta);
    virtual int getCorrectedAddPower(float targetDistance, float movedDistance);
    virtual void getCorrectedPower(int power, int *powerLeft, int *powerRight);
    virtual float getDistance(float distanceDelta);
    virtual float getDirection(float directionDelta);
    virtual bool runAsPattern(scenario_running scenario);
    virtual void initializeAsPattern(scenario_running scenario);
    virtual int addAdjustValue(int targetValue, int addvalue);
    virtual void getPowerForCurvatureRadius(enum runPattern pattern, float curvatureRadius, int power, int *powerLeft, int *powerRight);
    virtual void curveRun(enum runPattern pattern, int power, float curvatureRadius);
    virtual void jitteryMovementFromCoordinate(int power, float startX, float startY, float startDirection, float endX, float endY);
    virtual float distanceFromCoordinateForJitteryMovement(float startX, float startY, float endX, float endY);
    virtual float directionFromCoordinateForJitteryMovement(float startX, float startY, float startDirection, float endX, float endY);

#if FALSE //モデル図記載の式、うまくいかないので、ひとまず保留
    virtual float getCurvatureRadius(float startX, float startY, float startDirection, float endX, float endY, float endDirection, float _s);
    virtual void getOnceDifferential(float a1x, float a1y, float a2x, float a2y, float a3x, float a3y,float s, float *d1x, float *d1y);
    virtual void getSecondDifferential(float a2x, float a2y, float a3x, float a3y,float s, float *d2x, float *d2y);
    virtual float toVectorMagnitude(float x, float y);
    virtual float multiplicationVector(float x1, float y1, float x2, float y2);
    virtual void VectorFromDirection(float Direction, float *x, float *y);
    virtual float degForTrigonometric(float direction);
#endif //

    virtual float CalculationCurvatureRadius(float a0, float a1, float a2, float a3, float x);
    virtual float OnceDifferentialOfQuadraticFunction(float a1, float a2, float x);
    virtual float SecondDifferentialOfQuadraticFunction(float a2);
    virtual void smoothMovementFromCoordinate(scenario_coordinate _coordinateScenario);
    virtual float OnceDifferentialOfCubicFunction(float a1, float a2, float a3, float x);
    virtual float SecondDifferentialOfCubicFunction(float a2, float a3, float x);
    virtual int getDecelerationPower(int finishPower,  int runPower, float stopValue, float DecelerationRangeFromStopValue, float currentValue, bool softDeceleration);
    virtual int getAccelerationPower(int startPower, int runPower, float accelerationRange, float currentValue,bool softAcceleration);
    virtual int getSoftAccelAndDecelerationPower(int power, float stopValue, float currentValue, float accelerationRange,float decelerationRange, bool softAcceleration, bool softDeceleration);

private:
    void pinWheel(int power, float degree);
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

    //! ｘ座標の最後に指定した位置（テストでいれてみる）
    float positionTargetXLast;

        //! y座標の最後に指定した位置（テストでいれてみる）
    float positionTargetYLast;
    //@}

    //! 初期化済み判定フラグ
    bool    initialized;

    //! EV3が現在停止しているかどうか
    bool runningEV3;

    //! 利用可能かどうか
    bool    enabled;

    //! 加速はじめのパワー
    static const int accelStartPower = 10;

    //! 減速最後のパワー
    static const int  decelerationFinishPower=5;
};
