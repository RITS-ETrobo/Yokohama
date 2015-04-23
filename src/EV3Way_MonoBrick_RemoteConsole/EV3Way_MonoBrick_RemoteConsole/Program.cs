using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace EV3Way_MonoBrick_RemoteConsole
{
	class MainClass
	{
		// constant
		private const int SOCKET_PORT = 7360;
	
		public static void Main (string[] args)
		{
			try {
				// 指定されたサーバに接続
				Socket	sock   = new Socket(
					AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				sock.Connect("10.0.1.1", SOCKET_PORT);
	
				NetworkStream	conn = new NetworkStream(sock, true);
				SendCommandLoop(conn); // コンソール入力
				conn.Close();
			} catch (Exception e) {
				Console.Out.WriteLine("caught an exception: {0}", e.Message);
			}
		}

		private static void SendCommandLoop(NetworkStream connection) {
			Console.Out.WriteLine ("Please key g[start] s[stop] q[quit]:");

			bool	done = false;
			while (!done) {
				if (Console.KeyAvailable) {
					Console.Out.Write (">");
					ConsoleKeyInfo key = Console.ReadKey ();
					switch (key.KeyChar) {
					case 'g':
					case 's':
						// LeJOS 版に合わせてネットワークバイトオーダーで送信
						byte[] keyBytes = BitConverter.GetBytes ((int)key.KeyChar); // 4ByteArray (littele Endian)
						if (BitConverter.IsLittleEndian) {
							Array.Reverse (keyBytes); // little Endian -> big endian
						}
						connection.Write (keyBytes, 0, keyBytes.Length);
						Console.Out.WriteLine ();
						break;
					case 'q':
						done = true;
						break;
					}
				}

				RemoteReceiveTest (connection);

				// ロボット側の倒立振子制御ループが4msで回るので, それより短い値を指定.
				Thread.Sleep (2); // TODO: 適切な値か検証する
			}
		}

		/// <summary>
		/// Receive a string from EV3.
		/// </summary>
		/// <param name="connection">Connection.</param>
		private static void RemoteReceiveTest(NetworkStream connection)
		{

			// 受信
			try{
				if (connection.DataAvailable) {
					var buff = new byte[256];
					connection.Read(buff, 0, 1);
					int receiveBytes = buff[0];
					connection.Read(buff, 0, receiveBytes);
					// ネットワークバイトオーダー(big endian)で受信したため little endian に変換
					Array.Reverse(buff, 0, receiveBytes); // big endian -> little endian

					Console.Out.WriteLine(System.Text.Encoding.ASCII.GetString(buff, 0, receiveBytes));
				}
			}catch(Exception){
				return;
			}
			return;
		}
	}
}
