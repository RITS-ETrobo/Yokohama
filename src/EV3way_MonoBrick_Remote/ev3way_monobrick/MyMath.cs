using System;

namespace ETRobocon.EV3
{
	/// <summary>
	/// 俺の数学
	/// </summary>
	public class MyMath
	{
		/// <summary>
		/// 最大公約数を求めます。
		/// https://gist.github.com/ww24/5517384
		/// </summary>
		/// <param name="x">The x coordinate.</param>
		/// <param name="y">The y coordinate.</param>
		public static int GCD(int x, int y)
		{
			if (x < y)
			{
				x ^= y;
				y ^= x;
				x ^= y;
			}

			return y != 0  ? GCD(y, x % y) : x;
		}

		/// <summary>
		/// 最小公倍数を求めます。
		/// https://gist.github.com/ww24/5517384
		/// </summary>
		/// <param name="x">The x coordinate.</param>
		/// <param name="y">The y coordinate.</param>
		public static int LCM(int x, int y)
		{
			return x * y / (GCD (x, y));
		}

		public int y = 0;
		public int Sum(int x)
		{
			y += x;
			return y;
		}

		public void Reset()
		{
			y = 0;
		}

	}
}

