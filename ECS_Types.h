#pragma once
using ComponentType = unsigned int;
#define N_COMPONENT_TYPES 32

//TODO create Spawner Component.
enum class Component{
	Position = 0, SpriteSheet, State, Movable, Controller, Collidable, SoundEmitter, SoundListener,
	Vision, Health, Attacker, UI_Element,
};

//TODO create Spawner System.
enum class System{
	Renderer = 0, Movement, Collision, Control, State, SheetAnimation, Sound, Vision, Combat,
	Character_UI, Timers
};