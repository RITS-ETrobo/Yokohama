using System;

namespace ETRobocon.Utils
{
	/// <summary>
	/// 各種データ←→byte[] の変換を扱う.
	/// </summary>
	static public class PacketDataConverter
	{
		/// <summary>各種データからbyte配列への変換を行うデリゲート</summary>
		private delegate byte[] ConvertToPacketDataDel(object data);

		/// <summary>各種データからbyte配列への変換を行うデリゲートのインスタンスの配列.</summary>
		/// <remarks><see cref="PacketDataType"/>をインデックスに指定することを想定している.</remarks>
		private static ConvertToPacketDataDel[] _ConvertToPacketDataMethods = new ConvertToPacketDataDel[(byte)PacketDataType.NumOfType] {
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

		/// <summary>byte配列から各種データへの変換を行うデリゲート</summary>
		private delegate object ConvertFromPacketDataDel(byte[] data, byte dataCount);

		/// <summary>byte配列から各種データへの変換を行うデリゲートのインスタンスの配列</summary>
		/// <remarks><see cref="PacketDataType"/>をインデックスに指定することを想定している.</remarks>
		private static ConvertFromPacketDataDel[] _ConvertFromPacketDataMethods = new ConvertFromPacketDataDel[(byte)PacketDataType.NumOfType] {
			ConvertFromPacketDataDummy,
			ConvertPacketDataToBoolArray,
			ConvertPacketDataToSByteArray,
			ConvertPacketDataToShortArray,
			ConvertPacketDataToIntArray,
			ConvertPacketDataToLongArray,
			ConvertPacketDataToByteArray,
			ConvertPacketDataToUShortArray,
			ConvertPacketDataToUIntArray,
			ConvertPacketDataToULongArray,
			ConvertPacketDataToDecimalArray,
			ConvertPacketDataToCharArray,
			ConvertPacketDataToFloatArray,
			ConvertPacketDataToDoubleArray,
			ConvertPacketDataToString,
		};

		/// <summary>データ から byte[] への変換(エンコード)を行う</summary>
		/// <returns>変換されたbyte[]</returns>
		/// <param name="data">変換するデータ</param>
		/// <param name="type">変換するデータの型</param>
		public static byte[] ConvertToPacketData(object data, PacketDataType type)
		{
			if (data == null) {
				throw new ArgumentNullException();
			}
			if (type == PacketDataType.Invalid || (byte)type >= PacketDataType.NumOfType) {
				throw new ArgumentOutOfRangeException();
			}

			return _ConvertToPacketDataMethods[(byte)type](data);
		}

		/// <summary>byte[] から データへの変換(デコード)を行う</summary>
		/// <returns>変換されたデータ</returns>
		/// <param name="data">変換するbyte配列</param>
		/// <param name="dataCount"><paramref name="data"/>が含んでいるデータの数</param>
		/// <param name="type">変換するデータの型</param>
		public static object ConvertFromPacketData(byte[] data, byte dataCount, PacketDataType type)
		{
			if (data == null) {
				throw new ArgumentNullException();
			}
			if (dataCount == 0) {
				throw new ArgumentException();
			}
			if (type == PacketDataType.Invalid || (byte)type >= PacketDataType.NumOfType) {
				throw new ArgumentOutOfRangeException();
			}

			return _ConvertFromPacketDataMethods[(byte)type](data, dataCount);
		}

		#region Convert to PacketData methods

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

		#endregion

		#region Convert from PacketData methods

		private static object ConvertPacketDataToBoolArray(byte[] data, byte dataCount)
		{
			bool[] array = new bool[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToBoolean(data, i * PacketDataType.Boolean.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToSByteArray(byte[] data, byte dataCount)
		{
			sbyte[] array = new sbyte[dataCount];

			try {
				for (int i = 0; i < dataCount; i++) {
					array[i] = (sbyte)data[i];
				}
			}
			catch (InvalidCastException) {
				throw new ArgumentException();
			}
			return (object)array;
		}

		private static object ConvertPacketDataToShortArray(byte[] data, byte dataCount)
		{
			short[] array = new short[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToInt16(data, i * PacketDataType.Short.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToIntArray(byte[] data, byte dataCount)
		{
			int[] array = new int[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToInt32(data, i * PacketDataType.Int.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToLongArray(byte[] data, byte dataCount)
		{
			long[] array = new long[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToInt64(data, i * PacketDataType.Long.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToByteArray(byte[] data, byte dataCount)
		{
			return data.Clone();
		}

		private static object ConvertPacketDataToUShortArray(byte[] data, byte dataCount)
		{
			ushort[] array = new ushort[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToUInt16(data, i * PacketDataType.UShort.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToUIntArray(byte[] data, byte dataCount)
		{
			uint[] array = new uint[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToUInt32(data, i * PacketDataType.UInt.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToULongArray(byte[] data, byte dataCount)
		{
			ulong[] array = new ulong[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToUInt64(data, i * PacketDataType.ULong.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToDecimalArray(byte[] data, byte dataCount)
		{
			decimal[] array = new decimal[dataCount];
			for (int i = 0; i < dataCount; i++) {

				// そのdecimal値のint[]表現を求める
				int[] bits = new int[4];
				for (int bit = 0; bit < bits.Length; bit++) {
					bits[bit] = BitConverter.ToInt32(data,
						i * PacketDataType.Decimal.GetSize() + bit * PacketDataType.Int.GetSize());
				}

				// int[] -> decimal
				array[i] = new Decimal(bits);
			}
			return (object)array;
		}

		private static object ConvertPacketDataToCharArray(byte[] data, byte dataCount)
		{
			char[] array = new char[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToChar(data, i * PacketDataType.Char.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToFloatArray(byte[] data, byte dataCount)
		{
			float[] array = new float[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToSingle(data, i * PacketDataType.Float.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToDoubleArray(byte[] data, byte dataCount)
		{
			double[] array = new double[dataCount];
			for (int i = 0; i < dataCount; i++) {
				array[i] = BitConverter.ToDouble(data, i * PacketDataType.Double.GetSize());
			}
			return (object)array;
		}

		private static object ConvertPacketDataToString(byte[] data, byte dataCount)
		{
			return (object)System.Text.Encoding.ASCII.GetString(data, 0, dataCount);
		}

		private static object ConvertFromPacketDataDummy(byte[] data, byte dataCount)
		{
			throw new InvalidOperationException("This method must not be called.\n");
		}

		#endregion
	}
}

