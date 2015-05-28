using System;
using System.Net.Sockets;	// for NetworkStream

namespace ETRobocon.Utils
{
	/// <summary>EV3-PC間通信プロトコルに従って, EV3-PCの通信を行うためのクラス.</summary>
	/// <remarks>
	/// <para>シングルトンクラスとして利用する. EV3, PCそれぞれで一つずつインスタンスが存在する.</para>
	/// <para>EV3-PC間通信プロトコルについては, docs/design/misc/protocol.mdを参照のこと.</para>
	/// </remarks>
	public class ProtocolProcessor
	{
		/// <summary>ProtocolProcessorクラスのインスタンス</summary>
		/// <value>
		/// プログラム上で一つだけ存在するProtocolProcessorクラスのインスタンス.
		/// まだ通信確立済みのインスタンスを生成していないときは<c>null</c>.
		/// </value>
		/// <seealso cref="Connect"/>
		public static ProtocolProcessor Instance { get; private set; } = null;

		/// <summary><see cref="Instance"/>プロパティを排他制御するためのロック</summary>
		/// <remarks>複数タスクが同時にインスタンス生成をするのを防ぐため.</remarks>
		private static object _instanceLock = new object();

		/// <summary>通信用ストリーム</summary>
		/// <remarks>通信未確立のときは<c>null</c>となる.</remarks>
		private NetworkStream _stream = null;

		/// <summary><see cref="_stream"/>フィールドを排他制御するためのロック</summary>
		/// <remarks>複数タスクが同時に通信しようとするのを防ぐため.</remarks>
		private object _streamLock = new object();

		/// <summary>通信確立済みのインスタンスを生成するコンストラクタ.</summary>
		/// <param name="isEV3">EV3側が持つインスタンスを生成するなら<c>true</c>, PC側が持つインスタンスを生成するなら<c>false</c>.</param>
		private ProtocolProcessor(bool isEV3)
		{
			if (isEV3) {
				// PCとの接続
				/*
				NetworkStream connection;
				IPAddress ipAddr = IPAddress.Parse("10.0.1.1");

				var listener = new TcpListener (ipAddr, SOCKET_PORT); 
				listener.Start(); // クライアントからの受信接続要求の待機を開始

				var dialogCON = new InfoDialog ("Please connect...", false);
				dialogCON.Show(); // Wait for enter to be pressed

				try {
					Socket sock = listener.AcceptSocket(); // 接続要求の受け入れ
					connection = new NetworkStream(sock, true);
				} catch (SocketException) {
					var dialogE = new InfoDialog ("Connect is Failed.", false);
					dialogE.Show(); // Wait for enter to be pressed
					connection = null;
				}
				listener.Stop();
				return connection;
				*/
			}
			else {
				// EV3との接続
				/*
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
				*/
			}
		}

		/// <summary>通信を確立する</summary>
		/// <remarks>通信が既に確立していたら何もしない.</remarks>
		/// <param name="toEV3">通信相手がEV3なら<c>true</c>, PCなら<c>false</c>.</param>
		public static void Connect(bool toEV3)
		{
			throw new NotImplementedException();
		}

		/// <summary>通信相手へデータを送る</summary>
		/// <param name="data">送りたいデータ</param>
		public void SendData(object data)
		{
			throw new NotImplementedException();
		}

		/// <summary>通信相手からデータを受け取る.</summary>
		/// <remarks>受け取るデータが無かった場合でも待ち状態とならず, メソッドは終了する.</remarks>
		/// <returns>
		/// データを受信できた場合は<c>true</c>, 
		/// 受け取るデータが無かった場合は<c>false</c>.
		/// </returns>
		/// <param name="data">
		/// 受信したパケットの内容を表す<c>object</c>.
		/// ストリームに受信パケットが無かった場合は<c>null</c>となる.</param>
		public bool ReceiveData(out object data)
		{
			throw new NotImplementedException();
		}
	}
}

