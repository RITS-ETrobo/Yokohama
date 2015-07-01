using System;

namespace ETRobocon.Utils
{
	/// <summary>コマンド</summary>
	public class Command
	{
		/// <summary>このコマンドのID</summary>
		public CommandID Id { get; set; }

		/// <summary>このコマンドの第一パラメータの型</summary>
		/// <value>第一パラメータが無ければ<c>null</c>, それ以外はその型の<see cref="System.Type"/></value>
		public Type ParameterType1 { get; set; }

		/// <summary>このコマンドの第二パラメータの型</summary>
		/// <value>第二パラメータが無ければ<c>null</c>, それ以外はその型の<see cref="System.Type"/></value>
		public Type ParameterType2 { get; set; }

		public Command ()
		{
		}
	}
}
