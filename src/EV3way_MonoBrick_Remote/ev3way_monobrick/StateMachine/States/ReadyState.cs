using System;
using System.Threading;
using ETRobocon.EV3;
using ETRobocon.Utils;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.StateMachine
{
	public class ReadyState : State
	{
		public ReadyState(EV3body body) : base(body, 4)
		{
		}

		public override void Enter()
		{
			// Bluetooth関係のETロボコン拡張機能を有効にする
			Brick.InstallETRoboExt ();

			// コマンドタスク開始 & ログタスク開始
			// どちらも通信確立を行うが, どちらを先に呼び出しても良い.
			// (一方で通信確立すると, もう一方の通信確立処理はスキップされる.)
			// TODO: 通信確立のためのStateを作り, そちらでやらせる.
			CommandTask.Run();
			LogTask.Run();
			LogTask.Enable = true;

			// センサーおよびモータに対して初回アクセスをしておく
			_body.color.Read();
			_body.sonar.Read();
			_body.gyro.Read ();
			_body.motorL.SetPower (0);
			_body.motorR.SetPower (0);
			_body.motorT.SetPower (0);

			_body.motorL.ResetTacho ();
			_body.motorR.ResetTacho ();
			_body.motorT.ResetTacho ();
			Balancer.init ();

			var dialogSTART = new InfoDialog ("Touch to START", false);
			dialogSTART.Show ();

			LogTask.LogRemote("EV3 is ready.");
		}

		public override void Do()
		{
			tail_control(_body, TAIL_ANGLE_STAND_UP); //完全停止用角度に制御
		}

		public override void Exit()
		{
			// スイッチが離されるのを待つ
			// TODO: "押されたときだけを検出する"ような機能をタッチセンサーに持たせ, ここの処理は削除する.
			while (_body.touch.IsPressed ()) {
				tail_control(_body, TAIL_ANGLE_STAND_UP); //完全停止用角度に制御
				Thread.Sleep (4);
			}
		}

		public override TriggerID JudgeTransition()
		{
			if (_body.touch.IsPressed())
			{
				return TriggerID.TouchSensor;
			}

			return TriggerID.NoTrigger;
		}
	}
}

