using System;
using System.Net;	// for IPAddress
using System.Net.Sockets;	// for NetworkStream

namespace ETRobocon.Utils
{
	/// <summary>
	/// PacketDataTypeの拡張メソッド用のクラス.
	/// </summary>
	internal static class PacketDataTypeExt
	{
		/// <summary>サイズ取得用</summary>
		static readonly byte[] _sizes = new byte[] {
			0,	// sizes[0] is dummy.
			sizeof(bool),
			sizeof(sbyte),
			sizeof(short),
			sizeof(int),
			sizeof(long),
			sizeof(byte),
			sizeof(ushort),
			sizeof(uint),
			sizeof(ulong),
			sizeof(decimal),
			sizeof(char),
			sizeof(float),
			sizeof(double)
		};

		/// <summary><see cref="PacketDataType"/>のサイズを得る</summary>
		/// <returns>そのデータのサイズ(バイト)</returns>
		/// <param name="packetDataType">
		/// サイズを得たい型.
		/// <see cref="PacketDataType"/>のうち, Invalid, String, NumOfType以外が指定できる.
		/// </param>
		/// <exception cref="System.ArgumentOutOfRangeException">引数が不正.</exception>
		public static byte GetSize(this PacketDataType packetDataType)
		{
			byte typeNum = (byte)packetDataType;
			if (typeNum <= 0 || typeNum >= _sizes.Length) {
				throw new ArgumentOutOfRangeException("packetDataType", "This type is not supported.");
			}

			return _sizes[typeNum];
		}
	}

	/// <summary>送受信するデータの種別</summary>
	/// <remarks>パケットの1byte目にそのまま用いることを想定している.</remarks>
	internal enum PacketDataType : byte
	{
		Invalid,
		Boolean,
		SByte,
		Short,
		Int,
		Long,
		Byte,
		UShort,
		UInt,
		ULong,
		Decimal,
		Char,
		Float,
		Double,
		String,

		NumOfType
	};

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


		///	通信に用いるポート番号
		private const int SOCKET_PORT = 7360;

		/// <summary>各種データからbyte配列への変換を行うデリゲート</summary>
		private delegate byte[] ConvertToPacketData(object data);

		/// <summary>各種データからbyte配列への変換を行うデリゲートのインスタンスの配列.</summary>
		/// <remarks><see cref="PacketDataType"/>をインデックスに指定することを想定している.</remarks>
		private static ConvertToPacketData[] _ConvertToPacketDataMethods = new ConvertToPacketData[(byte)PacketDataType.NumOfType] {
			ConvertToPacketDataDummy,
			ConvertBoolToPacketData,
			ConvertSByteToPacketData,
			ConvertShortToPacketData,
			ConvertIntToPacketData,
			ConvertLongToPacketData,
			ConvertByteToPacketData,
			ConvertUShortToPacketData,
			ConvertUIntToPacketData,
			ConvertULongToPacketData,
			ConvertDecimalToPacketData,
			ConvertCharToPacketData,
			ConvertFloatToPacketData,
			ConvertDoubleToPacketData,
			ConvertStringToPacketData
		};

		/// <summary>通信確立済みのインスタンスを生成するコンストラクタ.</summary>
		/// <param name="isEV3">EV3側が持つインスタンスを生成するなら<c>true</c>, PC側が持つインスタンスを生成するなら<c>false</c>.</param>
		private ProtocolProcessor(bool isEV3)
		{
			if (isEV3) {
				// PCとの接続
				IPAddress ipAddr = IPAddress.Parse("10.0.1.1");

				var listener = new TcpListener (ipAddr, SOCKET_PORT); 
				listener.Start(); // クライアントからの受信接続要求の待機を開始

				try {
					Socket sock = listener.AcceptSocket(); // 接続要求の受け入れ
					_stream = new NetworkStream(sock, true);
				} catch (SocketException) {
					_stream = null;
				}
				listener.Stop();
			}
			else {
				// EV3との接続
				try {
					// 指定されたサーバに接続
					Socket	sock   = new Socket(
						AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
					sock.Connect("10.0.1.1", SOCKET_PORT);

					_stream = new NetworkStream(sock, true);
				} catch (Exception e) {
					_stream = null;
				}
			}
		}

		/// <summary>通信を確立する</summary>
		/// <remarks>通信が既に確立していたら何もしない.</remarks>
		/// <param name="toEV3">通信相手がEV3なら<c>true</c>, PCなら<c>false</c>.</param>
		public static void Connect(bool toEV3)
		{
			lock (_instanceLock) {	// 複数のInstanceを生成してしまうことのないよう
				if (Instance == null) {
					Instance = new ProtocolProcessor(!toEV3);
				}
			}
		}

		/// <summary>通信相手へデータを送る</summary>
		/// <param name="data">送りたいデータ</param>
		public void SendData(object data)
		{
			PacketDataType packetDataType;
			byte packetDataCount;
			byte[] packetData;

			if (data is Array) {
				Array arrayData = (Array)data;
				packetDataType = GetPacketDataType(arrayData.GetType().GetElementType());
				packetDataCount = (byte)arrayData.Length;
				packetData = new byte[packetDataCount * packetDataType.GetSize()];
				for (int i = 0; i < packetDataCount; i++) {
					_ConvertToPacketDataMethods[(byte)packetDataType](arrayData.GetValue(i)).CopyTo(packetData, i * packetDataType.GetSize());
				}
			}
			else if (data is string) {
				packetDataType = PacketDataType.String;
				packetData = _ConvertToPacketDataMethods[(byte)PacketDataType.String](data);
				packetDataCount = (byte)packetData.Length;
			}
			else {
				packetDataType = GetPacketDataType(data.GetType());
				packetDataCount = 1;
				packetData = _ConvertToPacketDataMethods[(byte)packetDataType](data);
			}

			// TODO: パケットを作成する

			// TODO: パケットを送る
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

		/// <summary>
		/// System.TypeからPacketDataTypeへの変換を行う.
		/// </summary>
		/// <returns>
		/// 変換された<see cref="PacketDataType"/>.
		/// 変換できなかったときは<see cref="PacketDataType.Invalid"/>. 
		/// </returns>
		/// <param name="type">変換したい<see cref="System.Type"/>インスタンス.</param>
		private static PacketDataType GetPacketDataType(Type type)
		{
			Type[] types = new Type[(byte)PacketDataType.NumOfType - 1] {
				typeof(bool),
				typeof(sbyte),
				typeof(short),
				typeof(int),
				typeof(long),
				typeof(byte),
				typeof(ushort),
				typeof(uint),
				typeof(ulong),
				typeof(decimal),
				typeof(char),
				typeof(float),
				typeof(double),
				typeof(string)
			};
			for (byte i = 0; i < types.Length; i++) {
				if (type.Equals (types [i])) {
					return (PacketDataType)(i + 1);
				}
			}
			return PacketDataType.Invalid;
		}

		private static byte[] ConvertBoolToPacketData(object data)
		{
			return BitConverter.GetBytes((bool)data);
		}

		private static byte[] ConvertSByteToPacketData(object data)
		{
			return new byte[1] { (byte)(sbyte)data };
		}

		private static byte[] ConvertShortToPacketData(object data)
		{
			return BitConverter.GetBytes((short)data);
		}

		private static byte[] ConvertIntToPacketData(object data)
		{
			return BitConverter.GetBytes((int)data);
		}

		private static byte[] ConvertLongToPacketData(object data)
		{
			return BitConverter.GetBytes((long)data);
		}

		private static byte[] ConvertByteToPacketData(object data)
		{
			return new byte[1] { (byte)data };
		}

		private static byte[] ConvertUShortToPacketData(object data)
		{
			return BitConverter.GetBytes((ushort)data);
		}

		private static byte[] ConvertUIntToPacketData(object data)
		{
			return BitConverter.GetBytes((uint)data);
		}

		private static byte[] ConvertULongToPacketData(object data)
		{
			return BitConverter.GetBytes((ulong)data);
		}

		private static byte[] ConvertDecimalToPacketData(object data)
		{
			int[] intData = decimal.GetBits((decimal)data);
			byte[] byteData = new byte[intData.Length * sizeof(int)];
			for (int i = 0; i < intData.Length; i++) {
				BitConverter.GetBytes(intData[i]).CopyTo(byteData, i * sizeof(int));
			}
			return byteData;
		}

		private static byte[] ConvertCharToPacketData(object data)
		{
			return BitConverter.GetBytes((char)data);
		}

		private static byte[] ConvertFloatToPacketData(object data)
		{
			return BitConverter.GetBytes((float)data);
		}

		private static byte[] ConvertDoubleToPacketData(object data)
		{
			return BitConverter.GetBytes((double)data);
		}

		private static byte[] ConvertStringToPacketData(object data)
		{
			return System.Text.Encoding.ASCII.GetBytes((string)data);
		}

		private static byte[] ConvertToPacketDataDummy(object data)
		{
			throw new InvalidOperationException("This method must not be called.\n");
		}
	}
}

