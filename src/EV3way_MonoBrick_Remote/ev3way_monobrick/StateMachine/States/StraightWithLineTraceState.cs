﻿using System;
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

		private readonly double TargetDistance;
		private double _startDistance;
		private double _endDistance;

		private LineDetector _ld;

		public StraightWithLineTraceState(EV3body body, double targetDistance) : base(body, 2)
		{
			TargetDistance = targetDistance;
			_ld = new LineDetectorOld (0, 45, LineDetector.LineEdge.Left, 50.0f, 0, 55.0f ); // TODO: kp, ki, kdの値を変えて調査してください。
		}

		public override void Enter()
		{
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
				turn = _ld.CalculateTurn(_body.color.ReadSensorValue());
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

