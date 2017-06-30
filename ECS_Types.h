//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_ECS_TYPES_H
#define COLDSPACE_ECS_TYPES_H

#define N_COMPONENT_TYPES 32
using ComponentType=unsigned int;

enum class Component {
    Position=0, SpriteSheet, State, Movable, Controller, Collidable
};

enum class System {
    Renderer=0, Movement, Collision, Control, State, SheetAnimation
};

#endif //COLDSPACE_ECS_TYPES_H
