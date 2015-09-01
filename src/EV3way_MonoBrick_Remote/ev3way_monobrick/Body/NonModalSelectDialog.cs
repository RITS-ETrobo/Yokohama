using System;
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

		public NonModalSelectDialog(SelectionType[] selections, string title, bool allowEsc)
			: base(selections, title, allowEsc)
		{
			IsShowing = false;
		}
	}
}

