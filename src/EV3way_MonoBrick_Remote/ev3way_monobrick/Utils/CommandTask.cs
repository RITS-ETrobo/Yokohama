using System;
using System.Threading;	// for Thread
using MonoBrickFirmware.Display.Dialogs;	// for InfoDialog

namespace ETRobocon.Utils
{
	/// <summary>コマンド受信・実行タスク</summary>
	public class CommandTask
	{
		/// <summary>コマンドタスクのインスタンス</summary>
		private static CommandTask _Instance = new CommandTask();

		/// <summary>コマンドタスクが実行されているスレッド</summary>
		private Thread _CommandThread;

		/// <summary>コマンドタスクのメインループのSleep時間</summary>
		private const int LOOP_INTERVAL = 16;

		/// <summary>コマンドの実装メソッドのデリゲート</summary>
		private delegate void CommandMethodDel(object parameter1, object parameter2);

		/// <summary>コマンドの実装メソッド</summary>
		/// <remarks><see cref="CommandID"/>と, コマンドの実装を結びつけるために利用する.</remarks>
		private CommandMethodDel[] _CommandMethods;

		private CommandTask()
		{
			// CommandIDの列挙順と対応させること
			_CommandMethods = new CommandMethodDel[(int)CommandID.NumOfCommand] {
				CommandRun,
				CommandStop,
				CommandLog
			};
		}

		/// <summary>コマンドタスクの開始</summary>
		public static void Run()
		{
			ProtocolProcessorForEV3.Connect();

			CommandTask._Instance._CommandThread = new Thread(CommandTask._Instance.Loop);
			CommandTask._Instance._CommandThread.Priority = ThreadPriority.Lowest;
			CommandTask._Instance._CommandThread.Start();
		}

		/// <summary>コマンドタスクのメインループ</summary>
		private void Loop()
		{
			while (true)
			{
				Thread.Sleep(LOOP_INTERVAL);
			}
		}

		private void CommandRun(object parameter1, object parameter2)
		{
			var dialogRun = new InfoDialog ("Run Command", false);
			dialogRun.Show ();
		}

		private void CommandStop(object parameter1, object parameter2)
		{
			var dialogRun = new InfoDialog ("Stop command", false);
			dialogRun.Show ();
		}

		private void CommandLog(object parameter1, object parameter2)
		{
			var dialogRun = new InfoDialog ("Log command", false);
			dialogRun.Show ();
		}
	}
}

