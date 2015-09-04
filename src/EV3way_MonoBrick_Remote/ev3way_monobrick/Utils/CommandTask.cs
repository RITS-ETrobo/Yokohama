﻿using System;
using System.Threading;	// for Thread
using MonoBrickFirmware.Display.Dialogs;	// for InfoDialog

using ETRobocon.EV3;	// for MainClass

namespace ETRobocon.Utils
{
	/// <summary>コマンド受信・実行タスク</summary>
	public class CommandTask
	{
		/// <summary>コマンドタスクのインスタンス</summary>
		private static CommandTask _Instance = new CommandTask();

		/// <summary>コマンドタスクが実行されているスレッド</summary>
		private Thread _CommandThread;

		/// <summary>コマンドタスクのメインループのSleep時間[ms]</summary>
		private const int LOOP_INTERVAL = 16;

		/// <summary>コマンドパラメータがEV3側に到着するのが遅れたときの待ち時間[ms]</summary>
		private const int RECEIVE_INTERVAL = 4;

		/// <summary>この回数だけパラメータの受信を試みる</summary>
		private const int RECEIVE_WAIT_COUNT_MAX = 5;

		/// <summary>コマンドの実装メソッドのデリゲート</summary>
		private delegate void CommandMethodDel(object parameter1, object parameter2);

		/// <summary>コマンドの実装メソッド</summary>
		/// <remarks><see cref="CommandID"/>と, コマンドの実装を結びつけるために利用する.</remarks>
		private CommandMethodDel[] _CommandMethods;

        /// <summary>コマンドの受信状況</summary>
        /// <remarks>
        /// 他クラスがコマンド受信状況を把握するため,
        /// <see cref="CommandReceiveFlags"/>インスタンスの参照を持っておく.
        /// </remarks>
        private CommandReceiveFlags _commandReceiveFlags;

		private CommandTask()
		{
			try {
				_commandReceiveFlags = CommandReceiveFlags.Instance;

				// CommandIDの列挙順と対応させること
				_CommandMethods = new CommandMethodDel[(int)CommandID.NumOfCommand] {
					CommandRun,
					CommandStop,
					CommandLog
				};
			}
			catch (Exception)
			{
				// TODO: ログファイルへの出力
			}
		}

		/// <summary>コマンドタスクの開始</summary>
		public static void Run()
		{
			try
			{
				ProtocolProcessorForEV3.Connect();

				CommandTask._Instance._CommandThread = new Thread(CommandTask._Instance.Loop);
				CommandTask._Instance._CommandThread.Priority = ThreadPriority.Lowest;
				CommandTask._Instance._CommandThread.Start();
			}
			catch (Exception)
			{
				// TODO: ログファイルへの出力
			}
		}

		/// <summary>コマンドタスクのメインループ</summary>
		private void Loop()
		{
			object data;
			int receive_count;

			try
			{
				while (true)
				{
					data = null;
					if (ProtocolProcessorForEV3.Instance.ReceiveData (out data))
					{
						Array array = (Array)data;
						object raw_command_id = array.GetValue(0);
						CommandID command_id = (CommandID)(int)raw_command_id;	// IDは必ずintで送られてくる

						CommandFormat command_format = CommandFormatManager.GetCommandFormat(command_id);

						object param1 = null;
						object param2 = null;

						// コマンドパラメータ受信
						if (command_format.ParameterType1 != null)
						{
							// パラメータ1の受信
							receive_count = 0;
							while (ProtocolProcessorForEV3.Instance.ReceiveData(out param1) == false)
							{
								Thread.Sleep(RECEIVE_INTERVAL);
								receive_count++;
								if (receive_count == RECEIVE_WAIT_COUNT_MAX)
								{
									// TODO: ログファイルに残す
									break;
								}
							}

							if (command_format.ParameterType2 != null)
							{
								// パラメータ2の受信
								receive_count = 0;
								while (ProtocolProcessorForEV3.Instance.ReceiveData(out param2) == false)
								{
									Thread.Sleep(RECEIVE_INTERVAL);
									receive_count++;
									if (receive_count == RECEIVE_WAIT_COUNT_MAX)
									{
										// TODO: ログファイルに残す
										break;
									}
								}
							}
						}

                        // コマンドを受信したフラグを立てる
                        _commandReceiveFlags.ReceiveCommand(command_format.Id);

						// コマンド実行
						_CommandMethods[(int)command_id](param1, param2);
					}

					Thread.Sleep(LOOP_INTERVAL);
				}
			}
			catch (Exception)
			{
				// TODO: ログファイルへの出力
			}
		}

		private void CommandRun(object parameter1, object parameter2)
		{
			// Nothing to do
		}

		private void CommandStop(object parameter1, object parameter2)
		{
			// Nothing to do
		}

		private void CommandLog(object parameter1, object parameter2)
		{
			var dialogRun = new InfoDialog ("Log command", false);
			dialogRun.Show ();
		}
	}
}
