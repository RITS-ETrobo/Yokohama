using System;

namespace ETRobocon.Utils
{
	/// <summary>全ての<see cref="Command"/>を管理するクラス</summary>
	public class CommandManager
	{
		/// <summary>このクラスのインスタンス</summary>
		private static CommandManager _Instance = new CommandManager();

		/// <summary>全コマンドリスト</summary>
		private Command[] _Commands;

		private CommandManager ()
		{
			// 全コマンドリストの作成
			// この配列の要素の順序は, CommandIDの列挙順と一致させること.
			_Commands = new Command[(int)CommandID.NumOfCommand] {
				new Command(CommandID.Run, null, null),
				new Command(CommandID.Stop, null, null),
				new Command(CommandID.Log, typeof(bool), null)
			};
		}

		/// <summary>指定したIDに対応するコマンドを取得する</summary>
		/// <returns>指定したIDを持つコマンド</returns>
		/// <param name="commandId">取得したいコマンドのID</param>
		public static Command GetCommand(CommandID commandId)
		{
			return CommandManager._Instance._Commands[(int)commandId];
		}
	}
}
