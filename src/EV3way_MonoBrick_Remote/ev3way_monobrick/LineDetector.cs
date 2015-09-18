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

		public const int InitWhite = 44;
		public const int InitBlack = 2;
		public const LineEdge InitEdge = LineEdge.Left;

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
			this.White = (White>100)?100:((White<0)?0:White);
			this.Black = (Black>100)?100:((Black<0)?0:Black);
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
			this.White = (White>100)?100:((White<0)?0:White);
			this.Black = (Black>100)?100:((Black<0)?0:Black);

			half = (this.White + this.Black) / 2;
			gradient = (float)((2 * MaxTurnAbs) / (this.White - this.Black));
		}

		int half = 0;
		float gradient = 0;

		/// <summary>
		/// Turn値を計算します。
		/// </summary>
		/// <returns>Turn値</returns>
		public override sbyte CalculateTurn( int colorValue )
		{
			return (sbyte)(((colorValue - half) * gradient ) * (sbyte)edge);
		}
	}



	/// <summary>
	/// 旧ロボコンで使用していたLineDetectorです。
	/// </summary>
	public class LineDetectorOld : LineDetector
	{
		const int BufferSize = 10;
		float lineThreshold;
		float[] ringBuffer = new float[BufferSize];

		public const float InitKp = 20.0f;
		public const float InitKi = 0.0f;
		public const float InitKd = 80.0f;
		float mKp = InitKp;
		float mKi = InitKi;
		float mKd = InitKd;
		int bufIndex = 0;

		/// <summary>
		/// LineDetectorのインスタンスを生成します。
		/// </summary>
		/// <param name="White">光センサが白と認識する値</param>
		/// <param name="Black">光センサが黒と認識する値</param>
		/// <param name="edge">線のどちらを読むか</param>
		/// <param name="MaxTurnAbs">Turn値の最大、最小値の絶対値</param>
		public LineDetectorOld ( int White, int Black, LineEdge edge, float Kp, float Ki, float Kd )
			: base (White, Black, edge, 0)
		{
			lineThreshold = Black + ((3 * (White - Black)) / 4);
			InitBuffer ();
			this.mKp = Kp;
			this.mKi = Ki;
			this.mKd = Kd;
		}

		/// <summary>LineDetectorクラスが持つwhite, black, lineThresholdの値を, キャリブした値に変更する</summary>
		/// <param name="CalibratedWhite">キャリブ済みの白の値</param>
		/// <param name="CalibratedBlack">キャリブ済みの黒の値</param>
		public void SetEachColorValue(int CalibratedWhite, int CalibratedBlack){
			this.White = (CalibratedWhite>100)?100:((CalibratedWhite<0)?0:CalibratedWhite);
			this.Black = (CalibratedBlack>100)?100:((CalibratedBlack<0)?0:CalibratedBlack);
			this.lineThreshold = this.Black + ((3 * (this.White - this.Black)) / 4);
		}

		/// <summary>
		/// mKp, mKi, mKd の値をセットする.
		/// </summary>
		/// <param name="Kp">新しく設定するKp値</param>
		/// <param name="Ki">新しく設定するKi値</param>
		/// <param name="Kd">新しく設定するKd値</param>
		public void SetPIDValue(float Kp, float Ki, float Kd){
			this.mKp = Kp;
			this.mKi = Ki;
			this.mKd = Kd;
		}

		/// <summary>
		/// 正規化した光センサの値を取得する
		/// </summary>
		/// <returns>正規化した光センサの値</returns>
		float lightValueNormalization( int colorValue )
		{
			float L = (float)colorValue;

			float P = (L - lineThreshold); // 偏差
			if(L < lineThreshold){ // 黒
				P = P / (lineThreshold - Black); // [-1.0, 1.0] の値に正規化
				P *= 2; // 黒線は細くハミ出やすいので強めてハミ出ないようにする。
			}
			else{ // 白
				P = P / (White - lineThreshold); // [-1.0, 1.0] の値に正規化
			}

			if(P > 1) P = 1;
			if(P < -1) P = -1;

			return P;
		}


		/// <summary>
		/// PID制御による操作量の計算
		/// </summary>
		/// <param name="P">比例成分</param>
		/// <returns>操作量</returns>
		float control(float P)
		{
			// Nothing but buffering (Ring bufffer)
			float pP = ringBuffer[bufIndex]; /*1つ前のP値*/
			ringBuffer[bufIndex] = P;  /*今回の値を次使えるようにbufに格納する*/
			bufIndex = (bufIndex + 1) % BufferSize;/*index2がMAX_BUFになったら初期化*/

			// Integral 積分対象
			// I += P;
			//float I = 0;
			//for(int i = 0; i < BufferSize; i++)
			//{
			//	I += ringBuffer[i];
			//}

			//Derivative
			float D = P - pP;

			float Y = (mKp * P) + /*(Ki * I) +*/ (mKd * D);

			return Y;
		}

		/// <summary>
		/// バッファを初期化する
		/// </summary>
		void InitBuffer()
		{
			for(int i = 0; i < BufferSize; i++)
			{
				ringBuffer[i] = 0;
			}
		}

		/// <summary>
		/// Turn値を計算します。
		/// </summary>
		/// <returns>Turn値</returns>
		public override sbyte CalculateTurn( int colorValue )
		{
			//正規化した光センサ値をPに格納する
			float P = lightValueNormalization( colorValue );

			//Pid制御
			float Y = control(P);

			//ラインの右側をトレースするか左側をトレースするかで旋回方向が決まる
			Y *= (float)edge;

			return (sbyte)Y;
		}
	}
}

