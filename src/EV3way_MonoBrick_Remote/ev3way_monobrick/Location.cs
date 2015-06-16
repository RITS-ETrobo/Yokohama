using System;

namespace ETRobocon.EV3
{
	/// <summary>
	/// 位置座標(x,y)を表す構造体
	///  - ロボットの位置を表すために使用する
	/// </summary>
	public struct Location
	{
		private double x;
		/// <summary>
		/// 位置座標xのset/getを行うproperty
		/// </summary>
		/// <value>位置座標x.</value>
		public double X{
			set{
				this.x = value;
			}
			get{
				return x;
			}
		}
			
		private double y;
		/// <summary>
		/// 位置座標yのset/getを行うproperty
		/// </summary>
		/// <value>位置座標y</value>
		public double Y{
			set{
				this.y = value;
			}
			get{
				return y;
			}
		}

		/// <summary>
		/// 位置座標(x,y)を表す構造体のコンストラクタ
		/// <see cref="ETRobocon.EV3.Location"/>
		/// </summary>
		/// <param name="prm_x">位置座標x.</param>
		/// <param name="prm_y">位置座標y.</param>
		public Location (double prm_x, double prm_y){
			x = prm_x;
			y = prm_y;
		}
	}
}

