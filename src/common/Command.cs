using System;

namespace ETRobocon.Utils
{
	/// <summary>コマンド</summary>
	public struct Command
	{
		/// <summary>このコマンドのID</summary>
		public CommandID Id;

		/// <summary>このコマンドの第一パラメータ</summary>
		public object Parameter1;

		/// <summary>このコマンドの第二パラメータ</summary>
		public object Parameter2;

		/// <summary>コマンドの作成</summary>
		/// <param name="id">コマンドのID</param>
		/// <param name="parameter1">第一パラメータ</param>
		/// <param name="parameter2">第二パラメータ</param>
		public Command(CommandID id, object parameter1, object parameter2)
		{
			Id = id;
			Parameter1 = parameter1;
			Parameter2 = parameter2;
		}
	}
}

