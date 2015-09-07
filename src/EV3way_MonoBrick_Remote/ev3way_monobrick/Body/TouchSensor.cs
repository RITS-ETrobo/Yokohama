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
	}
}

