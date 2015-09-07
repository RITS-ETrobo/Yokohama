using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.EV3
{
	public class GyroSensor
	{
		EV3GyroSensor gyroSensor;
		public GyroSensor (SensorPort inport, GyroMode velocity )
		{
			gyroSensor = new EV3GyroSensor (inport, velocity);
		}

		public int Read(){
			return gyroSensor.Read ();
		}
	}
}

