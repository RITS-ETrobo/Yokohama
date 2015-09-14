using System;
using System.Net.Sockets;

using MonoBrickFirmware.Movement;
using MonoBrickFirmware.Sensors;
using ETRobocon.Body;

using ETRobocon.Body;

namespace ETRobocon.EV3
{
	/// <summary>機体</summary>
	// EV3bodyは構造体だがメンバは全部参照型なので,
	// 構造体がコピーされても, 中身はたぶん同じデバイスを参照するはず.
	// TODO: EV3body構造体をやめ, モデル図のとおりに機体制御と機体情報のクラスを作成する.
	public struct EV3body{
		///	モータオブジェクト : 左
		public  Motor motorL;

		///	モータオブジェクト : 右
		public  Motor motorR;

		///	モータオブジェクト : 尻尾
		public MotorTail motorTail;

		///	センサーオブジェクト : タッチセンサー
		public  TouchSensor touch;

		///	センサーオブジェクト : 超音波センサー
		public  EV3UltrasonicSensor sonar;

		///	センサーオブジェクト : 色センサー
		public  ColorSensor color;

		///	センサーオブジェクト : ジャイロセンサー
		public GyroSensor gyro;

		/// <summary>自己位置</summary>
		public  ETRobocon.Odometry.Odometry odm;

		public static void init(ref EV3body body){
			body.motorL = new Motor (MotorPort.OutC);
			body.motorR = new Motor (MotorPort.OutB);
			body.motorTail = new MotorTail ();
			body.touch = new TouchSensor(SensorPort.In1); 
			body.sonar = new EV3UltrasonicSensor (SensorPort.In2, UltraSonicMode.Centimeter); // return [mm]
			body.color = new ColorSensor(SensorPort.In3);
			body.gyro = new GyroSensor (SensorPort.In4);

			///自己位置推定インスタンス作成
			body.odm = new ETRobocon.Odometry.Odometry( ETRobocon.Odometry.Odometry.AVAILABLE_LOG_FEATURE);
		}
	}
}

