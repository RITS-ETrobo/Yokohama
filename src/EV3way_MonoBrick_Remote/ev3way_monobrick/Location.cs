using System;

namespace ETRobocon.EV3
{
	public class Location
	{
		private double x;
		private double y;

		public Location ()
		{
			x = 0.0;
			y = 0.0;
		}

		public Location (double prm_x, double prm_y){
			x = prm_x;
			y = prm_y;
		}


		public void setX ( double prm_x){
			x = prm_x;
		}

		public void setY ( double prm_y){
			y = prm_y;
		}

		public double getX ( ){
			return x;
		}

		public double getY ( ){
			return y;
		}

	}
}

