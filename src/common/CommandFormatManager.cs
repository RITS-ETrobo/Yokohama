using System;

namespace ETRobocon.Utils
{
	/// <summary>全ての<see cref="CommandFormat"/>を管理するクラス</summary>
	public class CommandFormatManager
	{
		/// <summary>このクラスのインスタンス</summary>
		private static CommandFormatManager _Instance = new CommandFormatManager();

		/// <summary>全コマンドフォーマットのリスト</summary>
		private CommandFormat[] _CommandFormats;

		private CommandFormatManager ()
		{
			try
			{
				// 全コマンドフォーマットの作成
				// この配列の要素の順序は, CommandIDの列挙順と一致させること.
				_CommandFormats = new CommandFormat[(int)CommandID.NumOfCommand] {
					new CommandFormat(CommandID.Run, null, null),
					new CommandFormat(CommandID.Stop, null, null),
					new CommandFormat(CommandID.Log, typeof(bool), null)
				};
			}
			catch (Exception)
			{
				// TODO: ログファイルへの出力
			}
		}

		/// <summary>指定したIDに対応するコマンドのフォーマットを取得する</summary>
		/// <returns>指定したIDを持つコマンドのフォーマット</returns>
		/// <param name="commandId">フォーマットを取得したいコマンドのID</param>
		public static CommandFormat GetCommandFormat(CommandID commandId)
		{
			return CommandFormatManager._Instance._CommandFormats[(int)commandId];
		}
	}
}
