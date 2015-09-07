using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.EV3
{
	public class GyroSensor
	{
		EV3GyroSensor gyroSensor;
		private int sensorValue;
		private bool rapidChange;

		private const int BORDER_RAPID_CHANGE = 100;
		public GyroSensor (SensorPort inport, GyroMode velocity )
		{
			gyroSensor = new EV3GyroSensor (inport, velocity);
			sensorValue = 0;
			rapidChange = false;
		}

		public int GetSensorValue(){
			sensorValue = gyroSensor.Read ();
			if (sensorValue < -BORDER_RAPID_CHANGE || sensorValue > BORDER_RAPID_CHANGE) {
				rapidChange = true;
			}

			return sensorValue;
		}

		public bool GetRapidChange(){
			return rapidChange;
		}
	}
}

