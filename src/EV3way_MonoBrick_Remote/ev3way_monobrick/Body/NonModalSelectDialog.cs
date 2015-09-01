using System;
using System.Threading;	// for CancellationTokenSource
using System.Threading.Tasks;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.Body
{
	/// <summary>
	/// 非モーダルな選択用ダイアログ.
	/// 既存のMonoBrickのモーダルなSelectDialogを, 非モーダルにしたもの.
	/// </summary>
	public class NonModalSelectDialog<SelectionType> : SelectDialog<SelectionType>
	{
		/// <summary>このダイアログが表示されているかどうか</summary>
		/// <value>表示されているときは<c>true</c>, 表示されていないときは<c>false</c>.</value>
		public bool IsShowing { get; set; }

		/// <summary>モーダルダイアログを非モーダルとして使うための非同期タスク</summary>
		private Task _task;

		/// <summary>このダイアログを外部から閉じるためのCancellationTokenSource</summary>
		private CancellationTokenSource _cancellationTokenSource;

		public NonModalSelectDialog(SelectionType[] selections, string title, bool allowEsc)
			: base(selections, title, allowEsc)
		{
			IsShowing = false;
		}

		/// <summary>この非モーダル選択ダイアログを表示する</summary>
		/// <returns>成功時 : true, 既に表示されていた場合 : false</returns>
		public new bool Show()
		{
			if (!IsShowing)
			{
				IsShowing = true;

				_task = new Task(this.Run);
				_task.Start();

				return true;
			}
			else
			{
				return false;
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

