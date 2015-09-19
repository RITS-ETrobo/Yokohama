using System;
using MonoBrickFirmware.Display.Dialogs;
using ETRobocon.EV3;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class StraightWithLineTraceState : State
	{
		// MainClassのrun()より移してきた
		private int _batteryLevel;
		private int _counter = 0;
		private bool _alert = false;

		/// <summary>次のステートに移るまでの走行距離[mm]</summary>
		private readonly double TargetDistance;
		/// <summary>このステートの開始時点の累計走行距離[mm]</summary>
		private double _startDistance;
		/// <summary>このステートの終了時点の累計走行距離[mm]</summary>
		private double _endDistance;

		/// <summary>
		/// ループ時間調整用定数
		/// 1ループの中で行うダミーループ数を示す.
		/// </summary>
		private const int LOOP_DELAY = 29;

		/// <summary>ライントレースありの直線走行ステート</summary>
		/// <param name="targetDistance">次のステートに移るまでの走行距離[mm]</param>
		public StraightWithLineTraceState(EV3body body, double targetDistance) : base(body, 1)
		{
			TargetDistance = targetDistance;
		}

		public override void Enter()
		{
			// キャリブ値を, LineDetectorに設定する.
			_body.ld.SetEachColorValue (_body.color.BlackSensorValue, _body.color.WhiteSensorValue);

			// 電圧を取得
			_batteryLevel = Brick.GetVoltageMilliVolt();

			_startDistance = _body.odm.TotalMoveDistanceMM;
			_endDistance = _startDistance + TargetDistance;
		}

		public override void Do()
		{
			sbyte forward;
			sbyte turn;

			_body.motorTail.UpdateTailAngle ();

			if (++_counter >= 40/4) {
				_alert = sonar_alert (_body);
				_counter = 0;
			}
			if (_alert) {
				forward = 0;
				turn = 0;
			} else {
				forward = 50;
				turn = _body.ld.CalculateTurn(_body.color.ReadSensorValue());
			}

			int gyroNow = _body.gyro.GetSensorValue();
			int thetaL = _body.motorL.GetTachoCount();
			int theTaR = _body.motorR.GetTachoCount();
			sbyte pwmL, pwmR;
			Balancer.control (
				(float)forward, (float)turn, (float)gyroNow, (float)GYRO_OFFSET, (float)thetaL, (float)theTaR, (float)_batteryLevel,
				out pwmL, out pwmR
			);

			if (pwmL == 0) {
				_body.motorL.Brake();
			} else {
				_body.motorL.SetPower(pwmL);
			}
			if (pwmR == 0) {
				_body.motorR.Brake();
			} else {
				_body.motorR.SetPower(pwmR);
			}

			// 自己位置の更新
			_body.odm.update(_body.motorL.GetTachoCount(), _body.motorR.GetTachoCount());

			// ループ時間の調整
			for (int i = 0; i < LOOP_DELAY; i++) {
				double dummy;
				dummy = i / 100.1F;
				dummy = dummy * 0.0F;
				i = i + (int)dummy;
			}
		}

		public override void Exit()
		{
		}

		public override TriggerID JudgeTransition()
		{
			if (_body.gyro.GetRapidChange ()) {
				return TriggerID.DetectShock;
			}
			if (_body.touch.DetectReleased())
			{
				return TriggerID.TouchSensor;
			}
			if (CommandReceiveFlags.Instance.CheckCommandReceived(CommandID.Stop))
			{
				return TriggerID.StopCommand;
			}
			if (_body.odm.TotalMoveDistanceMM >= _endDistance)
			{
				return TriggerID.ReachDistance;
			}

			return TriggerID.NoTrigger;
		}
	}
}

