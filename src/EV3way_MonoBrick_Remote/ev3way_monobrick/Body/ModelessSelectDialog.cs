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

		/// <summary>モーダルダイアログをモーダレスとして使うための非同期タスク</summary>
		private Task _showModalDialogTask;

		/// <summary>このダイアログを外部から閉じるためのCancellationTokenSource</summary>
		private CancellationTokenSource _cancellationTokenSource;

		/// <summary>ダイアログを複数表示させないための排他制御のロック</summary>
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
			// IsShowingの判定と, IsShowingのtrueのセットの間に, 他タスクへ処理が切り替わると,
			// 複数のタスクでダイアログ表示処理が実行されてしまう可能性がある.
			// そのためIsShowingの判定からtrueのセットまでは, クリティカルセクション.
			lock (_lockObj)
			{
				if (IsShowing)
				{
					return false;
				}
				IsShowing = true;
			}

			_cancellationTokenSource = new CancellationTokenSource ();

			_showModalDialogTask = new Task(this.Run);
			_showModalDialogTask.Start();

			return true;
		}

		private void Run()
		{
			// モーダルダイアログを開く.
			// ダイアログは, ユーザによるボタン操作, もしくは
			// ModelessSelectDialog<SelectionType>インスタンスのCancel()を呼び出すことで閉じられる.
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

