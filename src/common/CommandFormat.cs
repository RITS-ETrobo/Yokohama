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
			Command parsedCommand = new Command(CommandID.Invalid, null, null);

			// パラメータが3個以上あるのは不正
			// (最初の要素にはコマンド名が入っているので, argvの要素数 = パラメータ数 + 1)
			if (argv.Length > 3)
			{
				return parsedCommand;
			}
			// パラメータをとるコマンドなのにパラメータがなければ不正
			if ((ParameterType1 != null && argv.Length < 2) || (ParameterType2 != null && argv.Length < 3))
			{
				return parsedCommand;
			}

			// パラメータ1
			if (argv.Length >= 2)
			{
				object parameter1 = CommandFormat.ParseStringWithType(argv[1], ParameterType1);
				if (parameter1 == null)
				{
					return parsedCommand;
				}
				parsedCommand.Parameter1 = parameter1;
			}

			// パラメータ2
			if (argv.Length >= 3)
			{
				object parameter2 = CommandFormat.ParseStringWithType(argv [2], ParameterType2);
				if (parameter2 == null)
				{
					return parsedCommand;
				}
				parsedCommand.Parameter2 = parameter2;
			}

			// ID
			parsedCommand.Id = this.Id;

			return parsedCommand;
		}

		/// <summary>文字列を指定した型に変換</summary>
		/// <returns>
		/// 型変換したものをボックス化した<c>object</c>.
		/// 型変換に失敗した場合は<c>null</c>.
		/// </returns>
		/// <param name="str">変換する文字列</param>
		/// <param name="type">変換先の型</param>
		private static object ParseStringWithType(string str, Type type)
		{
			if (str == null || type == null)
			{
				return null;
			}

			try {
				if (type.Equals (typeof(bool))) { return (object)Convert.ToBoolean(str); }
				else if (type.Equals (typeof(sbyte))) {	return (object)Convert.ToSByte(str); }
				else if (type.Equals (typeof(short))) { return (object)Convert.ToInt16(str); }
				else if (type.Equals (typeof(int))) { return (object)Convert.ToInt32(str); }
				else if (type.Equals (typeof(long))) { return (object)Convert.ToInt64(str); }
				else if (type.Equals (typeof(byte))) { return (object)Convert.ToByte(str); }
				else if (type.Equals (typeof(ushort))) { return (object)Convert.ToUInt16(str); }
				else if (type.Equals (typeof(uint))) { return (object)Convert.ToUInt32(str); }
				else if (type.Equals (typeof(ulong))) { return (object)Convert.ToUInt64(str); }
				else if (type.Equals (typeof(decimal))) { return (object)Convert.ToDecimal(str); }
				else if (type.Equals (typeof(char))) { return (object)Convert.ToChar(str); }
				else if (type.Equals (typeof(float))) { return (object)Convert.ToSingle(str); }
				else if (type.Equals (typeof(double))) { return (object)Convert.ToDouble(str); }
				else if (type.Equals (typeof(string))) { return (object)str; }
				else { return null; }
			}
			catch (FormatException)
			{
				return null;
			}
			catch (OverflowException)
			{
				return null;
			}
			catch (ArgumentNullException)
			{
				return null;
			}
		}
	}
}
