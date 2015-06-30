using System;
using System.Text;
using System.Threading;

using EV3Way_MonoBrick_RemoteConsole.Utils;

namespace EV3Way_MonoBrick_RemoteConsole
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			try {
				LogTask.Run();

				SendCommandLoop(); // コンソール入力
			} catch (Exception e) {
				Console.Out.WriteLine("caught an exception: {0}", e.Message);
			}
		}

		private static void SendCommandLoop() {
//			Console.Out.WriteLine ("Please key g[start] s[stop] q[quit]:");
			Console.Out.WriteLine("Push EV3 switch for [start][stop]. Or push key q for [quit] :");

			bool	done = false;
			while (!done) {
				if (Console.KeyAvailable) {	// ReadKey()でキー入力待ちになりその後のテキスト受信処理に進まなくなるのを回避
					Console.Out.Write (">");
					ConsoleKeyInfo key = Console.ReadKey ();
					switch (key.KeyChar) {
					case 'g':
					case 's':
						// PC→EV3はまだできていないため, コメントアウト.
						// 当分, 本体スイッチでstart/stopしてください.

//						// LeJOS 版に合わせてネットワークバイトオーダーで送信
//						byte[] keyBytes = BitConverter.GetBytes ((int)key.KeyChar); // 4ByteArray (littele Endian)
//						if (BitConverter.IsLittleEndian) {
//							Array.Reverse (keyBytes); // little Endian -> big endian
//						}
//						connection.Write (keyBytes, 0, keyBytes.Length);
//						Console.Out.WriteLine ();
						break;
					case 'q':
						done = true;
						break;
					}
				}

				// ロボット側の倒立振子制御ループが4msで回るので, それより短い値を指定.
				Thread.Sleep (2); // TODO: 適切な値か検証する
			}
		}
	}
}
