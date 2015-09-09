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

		/// <summary>
		/// 正常と判断するジャイロセンサー値の大きさの上限.
		/// 調査結果から, 正常走行時に, ジャイロセンサー値が3桁になる事態は想定できない.
		/// 詳細は, https://github.com/RITS-ETrobo/Yokohama/wiki/reports_GyroSensor を参照する事
		/// </summary>
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

			// ジャイロセンサー値が異常値を示したら, 急激な変化があったと判断する.
			// ジャイロセンサー値が正：前方に回転する加速
			// ジャイロセンサー値が負：後方に回転する加速
			// 前方への加速の大きさと後方への加速の大きさは同一である.
			if (sensorValue < -BORDER_RAPID_CHANGE || sensorValue > BORDER_RAPID_CHANGE) {
				rapidChange = true;
			}

			return sensorValue;
		}

		/// <summary>センサーが取得した値が異常値かどうかを返す</summary>
		public bool GetRapidChange(){
			return rapidChange;
		}
	}
}

