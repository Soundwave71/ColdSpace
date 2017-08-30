#pragma once

enum class EntityMessage{ 
	Move,
	Is_Moving,
	Frame_Change,
	State_Changed,
	Direction_Changed,
	Switch_State,
	Attack_Action,
	Dead,
	Target_Detected,
	Being_Attacked,
	Hurt,
	Die,
	Respawn,
	Removed_Entity,
	Attack,
	Mouse_Selection,
	UnSelect,
	Selection_Patrol
};