using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.Body
{
	/// <summary>
	/// MonoBrickFirmware.Sensors.EV3TouchSensor をラップし,
	/// タッチセンサ状態が変化したことを検知できるようにしたクラス.
	/// </summary>
	public class TouchSensor
	{
		/// <summary>既存のタッチセンサクラス</summary>
		private EV3TouchSensor _ev3TouchSensor;

		/// <summary>前回測定時の, 押下状態</summary>
		private bool _previousTouchState;

		public TouchSensor()
		{
			_ev3TouchSensor = new EV3TouchSensor(SensorPort.In1);
			_previousTouchState = false;
		}

		/// <summary>タッチセンサが離されたことを検出する</summary>
		/// <returns>タッチセンサが離されたことを検出したら<c>true</c>, 検出していないときは<c>false</c></returns>
		public bool DetectReleased()
		{
			bool now_touch_state = _ev3TouchSensor.IsPressed();

			// 前回の測定時は押されていて, 今回の測定時は押されていなかったときは, 離されたと判定.
			bool ret = (_previousTouchState == true && now_touch_state == false) ? true : false;

			_previousTouchState = now_touch_state;

			return ret;
		}
	}
}

