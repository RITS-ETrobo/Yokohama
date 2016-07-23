/**
 * @file    UltrasonicController.cpp
 * @brief   This file has UltrasonicController.
 *          
 */

#include "ev3api.h"
#include "utilities.h"
#include "pid_controller.h"
#include "app.h"
#include "SonarSensorController.h"


static bool enabledSonarSensor = true;

void setEnabledSonarSensor(bool _enabledSonarSensor){
    enabledSonarSensor = _enabledSonarSensor;
}

/**
 * @brief   超音波センサの初期化処理
 * 
 * @return  なし
*/
void initialize_sonarsensor() {
    ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);
}

/**
 * @brief   LEDの点灯（or消灯）処理
 *
 * @param   [in] color LEDの色（オフを含む）
 * @param   [in] duration 点灯(or消灯)時間[ms]
 * @return  なし
*/
void sensing_sonar(ledcolor_t color, TMO duration)
{
    ev3_led_set_color(color);
    if (duration > 0) {
        tslp_tsk(duration);
    }
}

/**
 * @brief   超音波の測定
 *
 * @return  なし
*/
void sensing_sonar(){
    //! タッチセンサーを押すと超音波で距離を測定
    if(ev3_touch_sensor_is_pressed(touch_sensor)){  
        int16_t distance= ev3_ultrasonic_sensor_get_distance(sonar_sensor);
        
        //! 距離[cm]*10を点滅間隔とする（最大3秒)
        TMO duration = 10 * distance;
        if (duration > 10 * 300) {
            duration = 10 * 300;
        }

        //! 超音波の受信の成功/失敗を判定
        if (duration > 0) {
            //! 距離が近いと短周期、遠いと長周期の点滅
            sensing_sonar(LED_RED, duration);
            sensing_sonar(LED_OFF, duration);
        }
    }
}

/**
 * @brief   超音波センサの制御
 * 
 * 超音波センサの参考資料 ： http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html
 *
 * @param   なし    
 * @return  なし
*/
void control_sonarsensor() {
    
    //! 超音波モードの準備ができたら音が2回鳴る
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_C4, 100);

    while(enabledSonarSensor){
        sensing_sonar();
        if(ev3_button_is_pressed(ENTER_BUTTON)){
            stop_emergency();
        }
    }
    setEnabledSonarSensor(true);
}