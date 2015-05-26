using NUnit.Framework;
using System;

namespace Test
{
	[TestFixture ()]
	public class Test
	{
		/// <summary>
		/// 18と24の最大公約数が6になるはず.
		/// </summary>
		[Test ()]
		public void GCDのテスト1_成功ケース ()
		{
			Assert.AreEqual(6, ETRobocon.EV3.MyMath.GCD (18, 24));
		}

		/// <summary>
		/// 6と8の最小公約数が24になるはず
		/// </summary>
		[Test ()]
		public void LCMのテスト1_成功ケース ()
		{
			Assert.AreEqual(24, ETRobocon.EV3.MyMath.LCM (6,8));
		}

		/// <summary>
		/// Sum(それまでの入力値をすべて加算して返すメソッド)のテスト。
		/// </summary>
		[Test ()]
		public void Sumのテスト1_成功ケース ()
		{
			ETRobocon.EV3.MyMath math = new ETRobocon.EV3.MyMath ();
			math.Sum (1);
			math.Sum (2);
			Assert.AreEqual( 6,math.Sum (3) );
		}

		/// <summary>
		/// Resetを実行すると、メンバ変数yが0になりますよね？
		/// </summary>
		[Test ()]
		public void Resetのテスト1_成功ケース ()
		{
			ETRobocon.EV3.MyMath math = new ETRobocon.EV3.MyMath ();
			math.Sum (1);
			math.Sum (2);
			math.Sum (3);
			math.Reset ();
			Assert.AreEqual (0, math.y);
		}
	}
}

