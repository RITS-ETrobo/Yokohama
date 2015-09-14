using System;
using ETRobocon.EV3;
using ETRobocon.Utils;

namespace ETRobocon.StateMachine
{
	public class ModeSelectState : State
	{
		/// <summary>このステートで表示する選択ダイアログ</summary>
		private Body.ModelessSelectDialog<string> _selectDialog;

		/// <summary>選択ダイアログのタイトル</summary>
		private string _dialogTitle;

		/// <summary>選択ダイアログの選択肢</summary>
		private string[] _dialogSelection;

		public ModeSelectState(EV3body body) : base(body, 4)
		{
			_dialogTitle = "welcome";
			_dialogSelection = new string[]
			{
				"Ready",
				"Calibrate"
			};
		}

		public override void Enter()
		{
			// TODO: show a modeless selection dialog
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
			if (_body.touch.DetectReleased())
			{
				return TriggerID.TouchSensor;
			}

			// TODO: return trigger for the selection

			return TriggerID.NoTrigger;
		}
	}
}
