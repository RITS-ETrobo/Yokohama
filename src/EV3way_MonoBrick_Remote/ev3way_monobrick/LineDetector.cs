using System;

namespace ETRobocon.EV3
{
	/// <summary>
	/// 線を検知して、左右の回転係数(どちらにどれくらいの速度で回転すればよいか)を生成するクラスです。
	/// これは、一番シンプルなもので、黒を検知したら
	/// </summary>
	public class LineDetector
	{
		/// 線のどちら側を読むか決定するenumです。
		public enum LineEdge:sbyte
		{
			/// 線の左側を読みます
			Left = -1,
			/// 線の右側を読みます
			Right = 1
		}

		/// ロボットがコース上の白色を検知したときのカラーセンサの値
		protected int White;

		/// ロボットがコース上の黒色を検知した時のカラーセンサの値
		protected int Black;

		/// 線のどちらの端をトレースするか
		protected LineEdge edge;

		/// Turn値の最大/最小値の絶対値
		protected sbyte MaxTurnAbs;

		/// <summary>
		/// LineDetectorのインスタンスを生成します。
		/// </summary>
		/// <param name="White">光センサが白と認識する値</param>
		/// <param name="Black">光センサが黒と認識する値</param>
		/// <param name="edge">線のどちらを読むか</param>
		/// <param name="MaxTurnAbs">Turn値の最大、最小値の絶対値</param>
		public LineDetector( int White, int Black, LineEdge edge, sbyte MaxTurnAbs )
		{
			this.White = White;
			this.Black = Black;
			this.edge = edge;
			this.MaxTurnAbs = MaxTurnAbs;
		}

		/// <summary>
		/// Turn値を計算します。
		/// Turn値はBalancer.control()に入力する回転係数のことをいいます.
		/// </summary>
		/// <returns>Turn値</returns>
		public virtual sbyte CalculateTurn( int colorValue )
		{
			return (colorValue >= ((Black + White) / 2)) ? (sbyte)(MaxTurnAbs * (sbyte)edge) : (sbyte)(-MaxTurnAbs * (sbyte)edge);
		}

	}

	/// <summary>
	/// P(Proposal)比例制御をするLineDetectorです。
	/// </summary>
	public class PLineDetector : LineDetector
	{
		/// <summary>
		/// LineDetectorのインスタンスを生成します。
		/// </summary>
		/// <param name="White">光センサが白と認識する値</param>
		/// <param name="Black">光センサが黒と認識する値</param>
		/// <param name="edge">線のどちらを読むか</param>
		/// <param name="MaxTurnAbs">Turn値の最大、最小値の絶対値</param>
		public PLineDetector ( int White, int Black, LineEdge edge, sbyte MaxTurnAbs)
			: base (White, Black, edge, MaxTurnAbs)
		{
			// nothing to do
		}

		/// <summary>
		/// Turn値を計算します。
		/// </summary>
		/// <returns>Turn値</returns>
		public override sbyte CalculateTurn( int colorValue )
		{
			return (sbyte)((-((2 * MaxTurnAbs) / (White + Black)) * colorValue + MaxTurnAbs) * (sbyte)edge);
		}
	}
}

