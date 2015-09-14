using System;
using ETRobocon.EV3;
using ETRobocon.Body;

namespace ETRobocon.StateMachine
{
	public class CalibrationModeState : State
	{
		/// <summary>キャリブレーションステートで表示される選択ダイアログ</summary>
		private ModelessSelectDialog<string> _selectDialog;

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
			_selectDialog = new ModelessSelectDialog<string>(_selections, _dialogTitle, false);
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
			if (!_selectDialog.IsShowing) {
				switch (_selectDialog.GetSelectionIndex ()) {
				case 0:
					return TriggerID.Select1;

				case 1:
					return TriggerID.Select2;

				case 2:
					return TriggerID.Select3;

				case 3:
					return TriggerID.Select4;

				default:
					// 何もしない
					break;
				}
			}

			return TriggerID.NoTrigger;
		}
	}
}
