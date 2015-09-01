using System;
using MonoBrickFirmware.Display.Dialogs;

namespace ETRobocon.EV3
{
	/// <summary>
	/// 非モーダルな選択用ダイアログ.
	/// 既存のMonoBrickのモーダルなSelectDialogを, 非モーダルにしたもの.
	/// </summary>
	public class NonModalSelectDialog<SelectionType> : SelectDialog<SelectionType>
	{
		public NonModalSelectDialog(SelectionType[] selections, string title, bool allowEsc)
			: base(selections, title, allowEsc)
		{
		}
	}
}

