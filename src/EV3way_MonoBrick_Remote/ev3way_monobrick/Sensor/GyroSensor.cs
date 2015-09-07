using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.EV3
{
	public class GyroSensor
	{
		EV3GyroSensor gyroSensor;
		private int sensorValue;
		private bool rapidChange;

		public GyroSensor (SensorPort inport, GyroMode velocity )
		{
			gyroSensor = new EV3GyroSensor (inport, velocity);
			sensorValue = 0;
			rapidChange = false;
		}

		public int GetSensorValue(){
			sensorValue = gyroSensor.Read ();
			return sensorValue;
		}
	}
}

