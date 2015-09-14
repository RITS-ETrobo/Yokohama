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
			_selectDialog = new Body.ModelessSelectDialog<string>(new string[]{"run", "go to CompleteState"}, "test", false);
			_selectDialog.Show();
		}

		public override void Do()
		{
			// Nothing to do
		}

		public override void Exit()
		{
			_selectDialog.Cancel();
		}

		public override TriggerID JudgeTransition()
		{
			if (_body.touch.DetectReleased())
			{
				return TriggerID.TouchSensor;
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
