using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class CalibrationModeState : State
	{
		/// <summary>キャリブレーションステートで表示される選択ダイアログの選択肢</summary>
		private string[] _selections;

		/// <summary>キャリブレーションステートで表示される選択ダイアログのタイトル</summary>
		private string _dialogTitle;

		public CalibrationModeState(EV3body body) : base(body, 4)
		{
			_selections = new string[]
			{
				"Calibrate White",
				"Calibrate Black",
				"Calibrate Gray",
				"Back"
			};

			_dialogTitle = "Calibration Mode";
		}

		public override void Enter()
		{
			// TODO: show a modeless select dialog
		}

		public override void Do()
		{
			// Nothing to do
		}

		public override void Exit()
		{
			// Nothing to do
		}

		public override TriggerID JudgeTransition()
		{
			// TODO: return trigger for the selection

			return TriggerID.NoTrigger;
		}
	}
}
