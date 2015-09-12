using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.Body
{
	/// <summary></summary>
	public class ColorSensor
	{
		/// <summary></summary>
		private EV3ColorSensor _ev3ColorSensor;



		/// <summary>カラーセンサーインスタンスの作成</summary>
		/// <param name="portSensorIn">カラーセンサーに対応する入力ポート</param>
		public ColorSensor(SensorPort portSensorIn)
		{
			_ev3ColorSensor = new EV3ColorSensor(portSensorIn);

		}

	}
}

