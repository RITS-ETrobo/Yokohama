using System;
using MonoBrickFirmware.Sensors;

namespace ETRobocon.Body
{
	/// <summary>
	/// カラーセンサーのAPIをラップしたクラス.
	/// センサー値の取得と, 黒, 白, 灰色のキャリブレーション値取得を行う.
	/// </summary>
	public class ColorSensor
	{
		/// <summary>カラーセンサーAPIのインスタンス</summary>
		private EV3ColorSensor _ev3ColorSensor;

		/// <summary>キャリブレーションした黒地の値を保持する.</summary>
		/// <value>キャリブレーションした黒地の値</value>
		public int BlackSensorValue {
			get;
			private set;
		}

		/// <summary>キャリブレーションした白地の値を保持する.</summary>
		/// <value>キャリブレーションした白地の値</value>
		public int WhiteSensorValue{
			get;
			private set;
		}

		/// <summary>キャリブレーションした灰色地の値を保持する.</summary>
		/// <value>キャリブレーションした灰色地の値</value>
		public int GraySensorValue{
			get;
			private set;
		}


		/// <summary>カラーセンサーインスタンスの作成</summary>
		/// <param name="portSensorIn">カラーセンサーに対応する入力ポート</param>
		public ColorSensor(SensorPort portSensorIn)
		{
			_ev3ColorSensor = new EV3ColorSensor(portSensorIn, ColorMode.Reflection);

			// https://github.com/RITS-ETrobo/Yokohama/wiki/Report_ColorSensor_by_angle
			// 上記URLを参考に, 初期値として設定.
			BlackSensorValue = 2;
			WhiteSensorValue = 37;
			GraySensorValue = 16;

		}

		/// <summary>カラーセンサーの値を取得する.</summary>
		/// <returns>取得したセンサー値</returns>
		public int ReadSensorValue(){
			return _ev3ColorSensor.Read ();
		}

		/// <summary>黒地キャリブレーション</summary>
		/// <returns>キャリブレーションされた黒地の値</returns>
		public int CalibrateBlack(){
			BlackSensorValue = ReadSensorValue ();
			return BlackSensorValue;
		}

		/// <summary>白地キャリブレーション</summary>
		/// <returns>キャリブレーションされた白地の値</returns>
		public int CalibrateWhite(){
			WhiteSensorValue = ReadSensorValue ();
			return WhiteSensorValue;
		}

		/// <summary>灰色地キャリブレーション</summary>
		/// <returns>キャリブレーションされた灰色地の値</returns>
		public int CalibrateGray(){
			GraySensorValue = ReadSensorValue ();
			return GraySensorValue;
		}

	}
}

