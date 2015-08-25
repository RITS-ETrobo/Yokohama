using System;
using System.Threading;
using System.Diagnostics;

using MonoBrickFirmware.Display.Dialogs;
using MonoBrickFirmware.Display;

using ETRobocon.EV3;
using ETRobocon.Odometry;
using ETRobocon.Utils;

///	2輪倒立振子ライントレースロボットの MonoBrick 用 c# プログラム
namespace ETRobocon.EV3
{
	class MainClass
	{
		//下記のパラメータは個体/環境に合わせて変更する必要があります
		///	ジャイロセンサオフセット値
		const int GYRO_OFFSET = 0;

		///	白色の光センサ値
		const int LIGHT_BLACK = 0;

		///	黒色の光センサ値
		const int LIGHT_WHITE = 40;

		///	超音波センサによる障害物検知距離 [mm]
		const int SONAR_ALERT_DISTANCE = 300;

		///	完全停止時の角度[deg]
		const int TAIL_ANGLE_STAND_UP = 94;

		///	バランス走行時の角度[deg]
		const int TAIL_ANGLE_DRIVE = 3;

		///	完全停止用モータ制御比例係数
		const float P_GAIN = 2.5F;

		///	完全停止用モータ制御PWM絶対最大値
		const int PWM_ABS_MAX = 60;

		///	リモートコマンド : ???
		const int REMOTE_COMMAND_CLOSE = 0;

		/// <summary>
		/// 自己位置推定
		/// </summary>
		static ETRobocon.Odometry.Odometry odm;

		/// <summary><see cref="RunCommandReceived"/>のフィールド</summary>
		private static bool _RunCommandReceived = false;
		/// <summary>runコマンドを受信したら立つフラグ</summary>
		public static bool RunCommandReceived
		{
			get{ return _RunCommandReceived; }
			set{ _RunCommandReceived = value; }
		}

		/// <summary><see cref="StopCommandReceived"/>のフィールド</summary>
		private static bool _StopCommandReceived = false;
		/// <summary>stopコマンドを受信したら立つフラグ</summary>
		/// <value><c>true</c> if stop command received; otherwise, <c>false</c>.</value>
		public static bool StopCommandReceived
		{
			get{ return _StopCommandReceived; }
			set{ _StopCommandReceived = value; }
		}

		public static void Main()
		{
			// 構造体の宣言と初期化
			var body = new EV3body ();
			EV3body.init (ref body);

			// Bluetooth関係のETロボコン拡張機能を有効にする
			Brick.InstallETRoboExt ();

			// コマンドタスク開始 & ログタスク開始
			// どちらも通信確立を行うが, どちらを先に呼び出しても良い.
			// (一方で通信確立すると, もう一方の通信確立処理はスキップされる.)
			CommandTask.Run();
			LogTask.Run();
			LogTask.Enable = true;

			// センサーおよびモータに対して初回アクセスをしておく
			body.color.Read();
			body.sonar.Read();
			body.gyro.Read ();
			body.motorL.SetPower (0);
			body.motorR.SetPower (0);
			body.motorT.SetPower (0);

			body.motorL.ResetTacho ();
			body.motorR.ResetTacho ();
			body.motorT.ResetTacho ();
			Balancer.init ();

			///自己位置推定インスタンス作成
			odm = new ETRobocon.Odometry.Odometry();

			// スタート待ち
			wait_start(body);

			var dialogRun = new InfoDialog ("Running", false);
			dialogRun.Show ();//Wait for enter to be pressed

			try{
				run(body);
			}catch(Exception){
				var dialogE = new InfoDialog ("Exception.", false);
				dialogE.Show();//Wait for enter to be pressed
			}

			body.motorL.Off ();
			body.motorR.Off ();
			body.motorT.Off ();

			Lcd.Instance.Clear ();
			Lcd.Instance.Update ();

			if (Debugger.IsAttached) {
				Brick.ExitToMenu (); // MonoBrickメインメニューへ戻る
			}
		}

		///	<summary>
		///	スタート待ち
		///	</summary>
		///	<returns>
		///	なし
		///	</returns>
		static void wait_start(EV3body body){
			var dialogSTART = new InfoDialog ("Touch to START", false);
			dialogSTART.Show (); // Wait for enter to be pressed

			LogTask.LogRemote("EV3 is ready.");

			while (!body.touch.IsPressed()) {
				tail_control(body, TAIL_ANGLE_STAND_UP); //完全停止用角度に制御
				if (MainClass.RunCommandReceived) {
					MainClass.RunCommandReceived = false;
					break;  // "run"コマンド受信
				}

				Thread.Sleep (4);
			}

			while (body.touch.IsPressed ()) {
				tail_control(body, TAIL_ANGLE_STAND_UP); //完全停止用角度に制御
				Thread.Sleep (4);
			}
		}

		static void run(EV3body body){
			// 電圧を取得
			int battery = Brick.GetVoltageMilliVolt();

			sbyte forward;
			sbyte turn;
			int counter = 0;
			bool alert = false;

			LogTask.LogRemote("EV3 run.");

			while (!body.touch.IsPressed ()) 
			{
				tail_control(body, TAIL_ANGLE_DRIVE); // バランス走行用角度に制御
				if (MainClass.StopCommandReceived) {
					MainClass.StopCommandReceived = false;
					break; // "stop"コマンド受信
				}

				if (++counter >= 40/4) {
					alert = sonar_alert (body);
					counter = 0;
				}
				if (alert) {
					forward = 0;
					turn = 0;
				} else {
					forward = 50;
					turn = (body.color.Read () >= (LIGHT_BLACK + LIGHT_WHITE) / 2) ? (sbyte)50 : (sbyte)-50;
				}

				int gyroNow = body.gyro.Read();
				int thetaL = body.motorL.GetTachoCount();
				int theTaR = body.motorR.GetTachoCount();
				sbyte pwmL, pwmR;
				Balancer.control (
					(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)theTaR, (float)battery,
					out pwmL, out pwmR
				);

				if (pwmL == 0) {
					body.motorL.Brake();
				} else {
					body.motorL.SetPower(pwmL);
				}
				if (pwmR == 0) {
					body.motorR.Brake();
				} else {
					body.motorR.SetPower(pwmR);
				}

				odm.update (body.motorL.GetTachoCount (), body.motorR.GetTachoCount ());

				// バランス制御のみだと3msecで安定
				// 尻尾制御と障害物検知を使用する場合2msecで安定
				// 上に加えて自己位置推定を入れても2msecで安定
				Thread.Sleep(2);
			}

			LogTask.LogRemote("EV3 stopped.");
		}

		///	<summary>
		///	超音波センサによる障害物検知
		///	</summary>
		///	<returns>
		/// - true : 障害物有り
		/// - false : 障害物無し
		/// </returns>
		/// <param name="body">
		///	EV3bodyのインスタンス 
		/// </param>
		static bool sonar_alert(EV3body body)
		{
			int distance = body.sonar.Read();
			if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0)){
				return true; /* 障害物を検知 */
			}else{
				return false; /* 障害物無し */
			}
		}

		///	<summary>
		///	走行体完全停止用モータの角度制御
		///	</summary>
		///	<returns>
		/// なし
		/// </returns>
		/// <param name="body">
		///	EV3bodyのインスタンス 
		/// </param>
		/// <param name="angle">
		///	モータ目標角度[度] 
		/// </param>
		static void tail_control(EV3body body, int angle)
		{
			float pwm = (float)(angle - body.motorT.GetTachoCount ()) * P_GAIN; // 比例制御
			// PWM出力飽和処理
			if (pwm > PWM_ABS_MAX) {
				pwm = PWM_ABS_MAX;
			} else if (pwm < -PWM_ABS_MAX) {
				pwm = -PWM_ABS_MAX;
			}
			if ((sbyte)pwm == 0) {
				body.motorT.Brake();
			} else {
				body.motorT.SetPower((sbyte)pwm);
			}
		}
	}
}

