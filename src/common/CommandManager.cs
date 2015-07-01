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
		}
	}
}

