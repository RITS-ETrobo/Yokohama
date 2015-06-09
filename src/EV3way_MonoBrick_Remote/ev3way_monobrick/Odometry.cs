using System;

namespace ETRobocon.EV3
{
	public class Odometry
	{
		private const double WHEEL_RADIUS_MM = 41.0; //車輪の半径[mm]
		private const double AXLE_LENGTH_MM = 16.0; //車軸の長さ[mm]

		private Location cur_location = new Location(0.0 , 0.0); //ロボットの現在地
		private int cur_right_encoder_deg = 0; //現在の右モーターのエンコーダー値[度]
		private int cur_left_encoder_deg = 0; //現在の左モーターのエンコーダー値[度]
		private double diff_right_encoder_rad = 0.0; //右エンコーダー値の差分[radian]
		private double diff_left_encoder_rad = 0.0; //左エンコーダー値の差分[radian]
		private double total_move_distance_mm = 0.0; //ロボットの累積走行距離[mm]
		private double cur_theta_rad = 0.0; //現在のロボットの旋回角度[radian]

		private readonly Object lock_obj = new Object();

		public Odometry ()
		{
		}

		/// <summary>
		/// 自己位置推定を実施する.
		/// </summary>
		/// <param name="args_left_tacho_count">左モーターのMotor.getTachoCount()の値[度].</param>
		/// <param name="args_right_tacho_count">右モーターのMotor.getTachoCount()の値[度].</param>
		public void update(int args_left_tacho_count , int args_right_tacho_count){

			lock(lock_obj){

				//前回の結果をローカル変数に格納
				Location prev_location = cur_location;
				int prev_right_encoder_deg = cur_right_encoder_deg;
				int prev_left_encoder_deg = cur_left_encoder_deg;
				double prev_theta_rad = cur_theta_rad;

				//現在のエンコーダー値[度]の更新
				cur_right_encoder_deg = args_right_tacho_count;
				cur_left_encoder_deg = args_left_tacho_count;

				//エンコーダー値の差分[radian]の算出
				diff_right_encoder_rad = degreeToRadian( cur_right_encoder_deg - prev_right_encoder_deg);
				diff_left_encoder_rad = degreeToRadian( cur_left_encoder_deg - prev_left_encoder_deg);

				//走行距離[mm]の算出
				double delta_right_move_distance_mm = WHEEL_RADIUS_MM * diff_right_encoder_rad; //右車輪走行距離増加分
				double delta_left_move_distance_mm = WHEEL_RADIUS_MM * diff_left_encoder_rad; //左車輪走行距離増加分
				double delta_move_distance_mm = (delta_right_move_distance_mm + delta_left_move_distance_mm) / 2.0; //ロボット全体の走行距離増加分
				total_move_distance_mm += delta_move_distance_mm; //累積走行距離[mm]に加算

				//ロボットの旋回角度増加分[radian]の算出
				double delta_theta_rad = (delta_right_move_distance_mm - delta_left_move_distance_mm) / AXLE_LENGTH_MM;

				//ロボットの旋回角度[radian]の更新
				cur_theta_rad = prev_theta_rad + delta_theta_rad;

				//ロボットの現在地の更新
				double trigono_func_arg = prev_theta_rad + delta_theta_rad / 2.0;
				double coefficient = delta_move_distance_mm * sinc_approx( delta_theta_rad / 2.0); 
				cur_location = new Location (
					prev_location.getX() + coefficient * Math.Cos (trigono_func_arg),
					prev_location.getY() + coefficient * Math.Sin (trigono_func_arg)
				);

			}
		}

		/// <summary>
		/// 度数法[度]から弧度法[radian]へ変換する.
		/// </summary>
		/// <returns>弧度法に直した値[radian].</returns>
		/// <param name="args_degree">度数法の値[度].</param>
		private double degreeToRadian (int args_degree){
			return args_degree * Math.PI / 180.0;
		}

		/// <summary>
		/// sinc関数の近似解をテイラー展開で求める.
		/// </summary>
		/// <returns>The approx.</returns>
		/// <param name="args_rad">sinc関数の引数[radian].</param>
		private double sinc_approx( double args_rad){

			double ret = 1.0;
			double rad_squared = Math.Pow (args_rad, 2);

			ret -= rad_squared / (3 * 2 * 1);
			ret += Math.Pow (rad_squared, 2) / (5 * 4 * 3 * 2 * 1);
			//ret -= Math.Pow (rad_squared, 3) / (7 * 6 * 5 * 4 * 3 * 2 * 1);
			//ret += Math.Pow (rad_squared, 4) / (9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
			//ret -= Math.Pow (rad_squared, 5) / (11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
			return ret;
		}

		/// <summary>
		/// ロボットの現在値を取得する.
		/// </summary>
		/// <returns>ロボットの現在値.</returns>
		public Location CurLocation{
			get{
				lock (lock_obj) {
					return this.cur_location;
				}
			}
		}

		/// <summary>
		/// ロボットの累積走行距離を取得する.
		/// </summary>
		/// <returns>累積走行距離[mm].</returns>
		public double TotalMoveDistanceMM{
			get{
				lock (lock_obj) {
					return this.total_move_distance_mm;
				}
			}
		}

	}
}

