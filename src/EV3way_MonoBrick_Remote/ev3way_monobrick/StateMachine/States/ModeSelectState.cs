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
			// Bluetooth関係のETロボコン拡張機能を有効にする
			Brick.InstallETRoboExt ();

			// コマンドタスク開始 & ログタスク開始
			// どちらも通信確立を行うが, どちらを先に呼び出しても良い.
			// (一方で通信確立すると, もう一方の通信確立処理はスキップされる.)
			// TODO: 今は起動時で接続を強制とするが, 通信確立のためのStateを作り, そちらでやらせる.
			CommandTask.Run();
			LogTask.Run();
			LogTask.Enable = true;

			_selectDialog = new Body.ModelessSelectDialog<string>(_dialogSelection, _dialogTitle, false);
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
