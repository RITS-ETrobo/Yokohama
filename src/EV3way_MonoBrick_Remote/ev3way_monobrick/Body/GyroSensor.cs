using System;
using MonoBrickFirmware.Sensors;
using ETRobocon.Utils;

namespace ETRobocon.Body
{
	public class GyroSensor
	{
		EV3GyroSensor gyroSensor;

		/// <summary>センサーの値を保持する</summary>
		private bool rapidChange;

		/// <summary>正常値の上限(-1倍すれば下限)</summary>
		private const int BORDER_RAPID_CHANGE = 180;

		/// <summary>ジャイロセンサーに対応するポートを設定する.</summary>
		/// <param name="portSensorIn">ジャイロセンサーに対応する入力ポート</param>
		public GyroSensor (SensorPort portSensorIn)
		{
			gyroSensor = new EV3GyroSensor (portSensorIn, GyroMode.AngularVelocity);
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
			}

			return sensorValue;
		}

		/// <summary>
		/// センサーが取得した値が異常値かどうかを返す.
		/// 異常値であれば, 衝撃を検知したと判断できる.
		/// </summary>
		/// <returns><c>true</c>, 異常値を取得していた(衝撃検知),
		///          <c>false</c> 正常値を取得していた(衝撃検知せず).</returns>
		public bool GetRapidChange(){
			return rapidChange;
		}
	}
}

