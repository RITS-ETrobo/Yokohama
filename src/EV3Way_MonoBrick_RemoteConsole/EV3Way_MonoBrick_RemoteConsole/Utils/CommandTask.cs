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

		/// <summary>プロンプト文字列</summary>
		private const string PROMPT = ">";

		private CommandTask()
		{
			try
			{
				// CommandIDの列挙順と対応させること
				_CommandName = new string[(int)CommandID.NumOfCommand] {
					"run",
					"stop",
					"log"
				};
			}
			catch(Exception e)
			{
				Console.Out.WriteLine("caught an exception: {0}", e.Message);
			}
		}

		/// <summary>コマンドタスクの開始</summary>
		public static void Run()
		{
			try
			{
				ProtocolProcessorForPC.Connect();

				CommandTask._Instance._CommandThread = new Thread(CommandTask._Instance.Loop);
				CommandTask._Instance._CommandThread.Priority = ThreadPriority.Lowest;
				CommandTask._Instance._CommandThread.Start();
			}
			catch (Exception e)
			{
				Console.Out.WriteLine("caught an exception: {0}", e.Message);
			}
		}

		/// <summary>コマンドタスクのメインループ</summary>
		private void Loop()
		{
			string str = "";

			Console.Write(PROMPT);

			while (true)
			{
				ConsoleKeyInfo key = Console.ReadKey();	// 待ち状態となる

				if (key.Key == ConsoleKey.Enter)
				{
					Console.Write('\n');

					// quit は特別処理
					if (str.Equals("quit"))
					{
						break;
					}

					Command command = ParseCommand(str);

					str = "";

					if (command.Id == CommandID.Invalid)
					{
						DisplayHelp();
						Console.Write(PROMPT);
						continue;
					}

					// 送信
					Console.WriteLine("sending command...:" + _CommandName[(int)command.Id]);

					ProtocolProcessorForPC.Instance.SendData((object)(int)command.Id);
					if (command.Parameter1 != null) {
						ProtocolProcessorForPC.Instance.SendData(command.Parameter1);
					}
					if (command.Parameter2 != null) {
						ProtocolProcessorForPC.Instance.SendData(command.Parameter2);
					}

					Console.Write(PROMPT);
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
			for (int i = 0; i < (int)CommandID.NumOfCommand; i++)
			{
				if (_CommandName[i].CompareTo(commandName) == 0)
				{
					return (CommandID)i;
				}
			}
			return CommandID.Invalid;
		}

		/// <summary>ユーザーが入力した文字列からコマンドへ変換する</summary>
		/// <returns>
		/// 文字列から変換されたコマンド.
		/// 変換できなかった場合は<see cref="Command.Id"/>が<see cref="CommandID.Invalid"/>となる.
		/// </returns>
		/// <param name="str">ユーザが入力した文字列</param>
		/// <exception cref="System.ArgumentNullException">引数が<c>null</c></exception>
		private Command ParseCommand(string str)
		{
			if (str == null)
			{
				throw new ArgumentNullException ("str");
			}

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

		/// <summary>コマンドの使い方(フォーマット)を表示</summary>
		private void DisplayHelp()
		{
			Console.WriteLine("usage: {");

			for (int i = 0; i < (int)CommandID.NumOfCommand; i++)
			{
				CommandFormat command_format = CommandFormatManager.GetCommandFormat((CommandID)i);

				// 名前
				Console.Write("\tname:" + _CommandName[i]);

				// パラメータ1
				if (command_format.ParameterType1 != null)
				{
					Console.Write("\tparameter1:" + command_format.ParameterType1.ToString());
				}

				// パラメータ2
				if (command_format.ParameterType2 != null)
				{
					Console.Write("\tparameter2:" + command_format.ParameterType2.ToString());
				}

				Console.Write('\n');
			}
			Console.WriteLine("\tname:quit\n}");
		}
	}
}

