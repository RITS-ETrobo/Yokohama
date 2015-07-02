﻿using System;
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
					str = "";
				}
				else
				{
					str += key.KeyChar;
				}
			}
		}
	}
}

