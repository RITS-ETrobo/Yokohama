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

		/// <summary>文字列の配列から, このコマンドのフォーマットに従って<see cref="Command"/>を作成する.</summary>
		/// <returns>
		/// 変換された<see cref="Command"/>.
		/// 変換が失敗すると, <see cref="Command.Id"/>が<see cref="CommandID.Invalid"/>に設定される.
		/// </returns>
		/// <param name="argv">ユーザが入力したコマンド名, およびパラメータを表す文字列</param>
		public Command ParseStringToCommand(string[] argv)
		{
			throw new NotImplementedException();
			Command parsedCommand = new Command(CommandID.Invalid, null, null);

			// パラメータが2個以上あるのは不正
			if (argv.Length > 3)
			{
				return parsedCommand;
			}

			// パラメータ1
			if (argv.Length >= 2)
			{
				// TODO: パラメータの変換
				if (!true)
				{
					return parsedCommand;
				}
			}

			// パラメータ2
			if (argv.Length >= 3)
			{
				// TODO: パラメータの変換
				if (!true)
				{
					return parsedCommand;
				}
			}

			// ID
			parsedCommand.Id = this.Id;

			return parsedCommand;
		}
	}
}
