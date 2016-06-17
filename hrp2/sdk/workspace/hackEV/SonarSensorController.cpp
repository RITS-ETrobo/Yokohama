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


//! \addtogroup 超音波センサのポート
//@{
//! Sonar sensor    :   Port 3
const sensor_port_t sonar_sensor = EV3_PORT_3;

//@}




/**
 * @brief   超音波センサの初期化処理
 *
 * @return  なし
*/
void initialize_sonarsensor() {
    ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);
}

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
    //! タッチセンサーを押すと超音波測定
    if(ev3_touch_sensor_is_pressed(touch_sensor)){  
        int16_t distance= ev3_ultrasonic_sensor_get_distance(sonar_sensor);
        
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
    ev3_speaker_play_tone(NOTE_C4, 100);
    tslp_tsk(200);
    ev3_speaker_play_tone(NOTE_C4, 100);

    while(1){
        sensing_sonar();
    }

    ev3_led_set_color(LED_OFF);
}