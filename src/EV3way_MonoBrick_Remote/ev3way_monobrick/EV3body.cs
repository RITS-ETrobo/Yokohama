using System;
using System.Net.Sockets;

using MonoBrickFirmware.Movement;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.EV3
{
	struct EV3body{
		///	モータオブジェクト : 左
		public  Motor motorL;

		///	モータオブジェクト : 右
		public  Motor motorR;

		///	モータオブジェクト : 尻尾
		public MotorTail motorTail;

		///	センサーオブジェクト : タッチセンサー
		public  EV3TouchSensor touch;

		///	センサーオブジェクト : 超音波センサー
		public  EV3UltrasonicSensor sonar;

		///	センサーオブジェクト : 色センサー
		public  EV3ColorSensor color;

		///	センサーオブジェクト : ジャイロセンサー
		public  EV3GyroSensor gyro;

		public static void init(ref EV3body body){
			body.motorL = new Motor (MotorPort.OutC);
			body.motorR = new Motor (MotorPort.OutB);
			body.motorTail = new MotorTail ();

			body.touch = new EV3TouchSensor (SensorPort.In1); 
			body.sonar = new EV3UltrasonicSensor (SensorPort.In2, UltraSonicMode.Centimeter); // return [mm]
			body.color = new EV3ColorSensor (SensorPort.In3, ColorMode.Reflection);
			body.gyro = new EV3GyroSensor (SensorPort.In4,	GyroMode.AngularVelocity);
		}
	}
}

