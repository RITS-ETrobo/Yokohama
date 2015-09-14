using System;
using System.Threading;
using ETRobocon.EV3;
using ETRobocon.Utils;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.StateMachine
{
	public class ReadyState : State
	{
		private Body.ModelessSelectDialog<string> _selectDialog;

		public ReadyState(EV3body body) : base(body, 4)
		{
		}

		public override void Enter()
		{
			// センサーおよびモータに対して初回アクセスをしておく
			_body.color.ReadSensorValue();
			_body.sonar.Read();
			_body.gyro.GetSensorValue ();
			_body.motorL.SetPower (0);
			_body.motorR.SetPower (0);

			_body.motorL.ResetTacho ();
			_body.motorR.ResetTacho ();

			Balancer.init ();

			_selectDialog = new Body.ModelessSelectDialog<string>(new string[]{"run", "go to CompleteState"}, "test", false);
			_selectDialog.Show();

			LogTask.LogRemote("EV3 is ready.");

			_body.motorTail.SetMotorAngle (MotorTail.TAIL_ANGLE_STAND_UP);	//完全停止用角度に制御
		}

		public override void Do()
		{
			_body.motorTail.UpdateTailAngle ();
		}

		public override void Exit()
		{
			_selectDialog.Cancel();
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
			if (CommandReceiveFlags.Instance.CheckCommandReceived(CommandID.Run))
			{
				return TriggerID.RunCommand;
			}
			if (!_selectDialog.IsShowing) {
				switch (_selectDialog.GetSelectionIndex ()) {
				case 0:
					return TriggerID.Select1;

				case 1:
					return TriggerID.Select2;

				default:
					// 何もしない
					break;
				}
			}

			return TriggerID.NoTrigger;
		}
	}
}

