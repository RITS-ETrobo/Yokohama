using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.Body
{
	public class TouchSensor
	{
		/// <summary>既存のタッチセンサクラス</summary>
		private EV3TouchSensor _ev3TouchSensor;

		public TouchSensor()
		{
			_ev3TouchSensor = new EV3TouchSensor(SensorPort.In1);
		}
	}
}

