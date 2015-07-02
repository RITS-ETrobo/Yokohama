using System;

namespace ETRobocon.Utils
{
	/// <summary>コマンドのフォーマット</summary>
	public class CommandFormat
	{
		/// <summary>このコマンドのID</summary>
		public CommandID Id { get; private set; }

		/// <summary>このコマンドの第一パラメータの型</summary>
		/// <value>第一パラメータが無ければ<c>null</c>, それ以外はその型の<see cref="System.Type"/></value>
		public Type ParameterType1 { get; private set; }

		/// <summary>このコマンドの第二パラメータの型</summary>
		/// <value>第二パラメータが無ければ<c>null</c>, それ以外はその型の<see cref="System.Type"/></value>
		public Type ParameterType2 { get; private set; }

		/// <summary>コマンドフォーマットの作成</summary>
		/// <param name="commandId">このコマンドのID</param>
		/// <param name="parameterType1">このコマンドの第一パラメータの型. 第一パラメータが無ければ<c>null</c>.</param>
		/// <param name="parameterType2">このコマンドの第二パラメータの型. 第二パラメータが無ければ<c>null</c>.</param>
		public CommandFormat(CommandID commandId, Type parameterType1, Type parameterType2)
		{
			Id = commandId;
			ParameterType1 = parameterType1;
			ParameterType2 = parameterType2;
		}
	}
}
