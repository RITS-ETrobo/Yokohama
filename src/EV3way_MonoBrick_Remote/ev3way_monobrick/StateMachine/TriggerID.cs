using System;

namespace ETRobocon.StateMachine
{
	///	<summary>走行に関するTrigger</summary>
	///	<remarks>
	///	Triggerを変更した時は、同時に
	///	$\src\EV3way_MonoBrick_Remote\ev3way_monobrick\StateMachine\StateMachine.cs
	///	の
	///	TransitionMatrix = new Transition?[(int)StateID.NumOfState, (int)TriggerID.NumOfTrigger]
	///	を変更する必要がある
	///
	///	なお、詳細な説明は、
	///	$\docs\HowToMakeStates.md
	///	を参照する事
	///	</remarks>
	public enum TriggerID
	{
		NoTrigger = -1,
		TouchSensor,
		ReachDistance,
		RunCommand,
		StopCommand,
		DetectShock,
		Select1,
		Select2,
		Select3,
		Select4,

		NumOfTrigger
	}
}

