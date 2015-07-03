using System;
using System.Threading;	// for Thread

using ETRobocon.Utils;	// for ProtocolProcessorForPC

namespace EV3Way_MonoBrick_RemoteConsole.Utils
{
	/// <summary>コマンド入力・送信タスク</summary>
	public class CommandTask
	{
		/// <summary>コマンドタスクのインスタンス</summary>
		private static CommandTask _Instance = new CommandTask();

		/// <summary>コマンドタスクが実行されているスレッド</summary>
		private Thread _CommandThread;

		/// <summary>コマンドの名前</summary>
		/// <remarks><see cref="CommandID"/>と, コマンドの名前を結びつけるために利用する.</remarks>
		private string[] _CommandName;

		private CommandTask()
		{
			// CommandIDの列挙順と対応させること
			_CommandName = new string[(int)CommandID.NumOfCommand] {
				"run",
				"stop",
				"log"
			};
		}

		/// <summary>コマンドタスクの開始</summary>
		public static void Run()
		{
			ProtocolProcessorForPC.Connect();

			CommandTask._Instance._CommandThread = new Thread(CommandTask._Instance.Loop);
			CommandTask._Instance._CommandThread.Priority = ThreadPriority.Lowest;
			CommandTask._Instance._CommandThread.Start();
		}

		/// <summary>コマンドタスクのメインループ</summary>
		private void Loop()
		{
			string str = "";

			while (true)
			{
				ConsoleKeyInfo key = Console.ReadKey();	// 待ち状態となる

				if (key.Key == ConsoleKey.Enter)
				{
					Console.Write('\n');

					Command command = ParseCommand(str);

					str = "";

					if (command.Id == CommandID.Invalid)
					{
						// TODO: usageの表示
						continue;
					}
				}
				else
				{
					str += key.KeyChar;
				}
			}
		}

		/// <summary>文字列からコマンドIDへ変換する</summary>
		/// <returns>
		/// コマンド名に対応する<see cref="CommandID"/>.
		/// 対応するものが無ければ, <see cref="CommandID.Invalid"/>
		/// </returns>
		/// <param name="commandName">コマンドの名前</param>
		private CommandID ConvertStringToId(string commandName)
		{
			throw new NotImplementedException();
		}

		/// <summary>ユーザーが入力した文字列からコマンドへ変換する</summary>
		/// <returns>
		/// 文字列から変換されたコマンド.
		/// 変換できなかった場合は<see cref="Command.Id"/>が<see cref="CommandID.Invalid"/>となる.
		/// </returns>
		/// <param name="str">ユーザが入力した文字列</param>
		private Command ParseCommand(string str)
		{
			string[] argv = str.Split(null);

			// このコマンド名に対応するコマンドIDを取得
			CommandID command_id;
			command_id = ConvertStringToId(argv[0]);
			if (command_id == CommandID.Invalid)
			{
				return new Command(CommandID.Invalid, null, null);
			}
				
			// そのコマンドのフォーマットに従って解析
			return CommandFormatManager.GetCommandFormat(command_id).ParseStringToCommand(argv);
		}
	}
}

