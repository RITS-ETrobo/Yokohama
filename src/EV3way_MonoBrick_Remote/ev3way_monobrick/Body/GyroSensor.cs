﻿using System;
using MonoBrickFirmware.Sensors;
using ETRobocon.Utils;

namespace ETRobocon.Body
{
	public class GyroSensor
	{
		EV3GyroSensor gyroSensor;

		/// <summary>
		/// GetSensorValueメソッドで衝撃を検知したかどうかを保持する.
		/// true:衝撃を検知した.  false:衝撃を検知していない.
		/// </summary>
		private bool rapidChange;

		/// <summary>正常値の上限(-1倍すれば下限)</summary>
		private const int BORDER_RAPID_CHANGE = 180;

		public GyroSensor (SensorPort inport, GyroMode velocity )
		{
			gyroSensor = new EV3GyroSensor (inport, velocity);
			rapidChange = false;
		}

		/// <summary>ジャイロセンサーの値を返す. また, 値が異常値かどうか判断する.</summary>
		/// <returns>ジャイロセンサーの値</returns>
		public int GetSensorValue(){
			int sensorValue;
			sensorValue = gyroSensor.Read ();

			// 値が異常値を示したら, 急激な変化があったと判断する.
			if (sensorValue < -BORDER_RAPID_CHANGE || sensorValue > BORDER_RAPID_CHANGE) {
				rapidChange = true;
			} else {
				rapidChange = false;
			}

			return sensorValue;
		}

		/// <summary>センサーが取得した値が異常値かどうかを返す</summary>
		public bool GetRapidChange(){
			return rapidChange;
		}
	}
}

