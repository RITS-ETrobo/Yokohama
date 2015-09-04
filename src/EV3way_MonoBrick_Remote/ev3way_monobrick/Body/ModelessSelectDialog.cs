using System;
using System.Threading;	// for CancellationTokenSource
using System.Threading.Tasks;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.Body
{
	/// <summary>
	/// モーダレスな選択用ダイアログ.
	/// 既存のMonoBrickのモーダルなSelectDialogを, モーダレスにしたもの.
	/// </summary>
	public class ModelessSelectDialog<SelectionType> : SelectDialog<SelectionType>
	{
		/// <summary>このダイアログが表示されているかどうか</summary>
		/// <value>表示されているときは<c>true</c>, 表示されていないときは<c>false</c>.</value>
		public bool IsShowing { get; set; }

<<<<<<< HEAD:src/EV3way_MonoBrick_Remote/ev3way_monobrick/Body/NonModalSelectDialog.cs
		/// <summary>モーダルダイアログを非モーダルとして使うための非同期タスク</summary>
		private Task _showModalDialogTask;
=======
		/// <summary>モーダルダイアログをモーダレスとして使うための非同期タスク</summary>
		private Task _task;
>>>>>>> origin/feature/230:src/EV3way_MonoBrick_Remote/ev3way_monobrick/Body/ModelessSelectDialog.cs

		/// <summary>このダイアログを外部から閉じるためのCancellationTokenSource</summary>
		private CancellationTokenSource _cancellationTokenSource;

		/// <summary>排他制御のためのロック</summary>
		private object _lockObj = new object();

		public ModelessSelectDialog(SelectionType[] selections, string title, bool allowEsc)
			: base(selections, title, allowEsc)
		{
			IsShowing = false;
		}

		/// <summary>このモーダレス選択ダイアログを表示する</summary>
		/// <returns>成功時 : true, 既に表示されていた場合 : false</returns>
		public new bool Show()
		{
			lock (_lockObj)
			{
				if (!IsShowing)
				{
					IsShowing = true;
				
					_cancellationTokenSource = new CancellationTokenSource ();

					_showModalDialogTask = new Task(this.Run);
					_showModalDialogTask.Start();

					return true;
				}
				else
				{
					return false;
				}
			}
		}

		private void Run()
		{
			base.Show(_cancellationTokenSource.Token);

			IsShowing = false;
		}

		/// <summary>このダイアログを閉じる</summary>
		public void Cancel()
		{
			if (IsShowing)
			{
				_cancellationTokenSource.Cancel();
			}
		}
	}
}

