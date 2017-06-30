//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_ENTITYEVENTS_H
#define COLDSPACE_ENTITYEVENTS_H


enum class EntityEvent{
    Spawned, Despawned, Colliding_X, Colliding_Y,
    Moving_Left, Moving_Right, Moving_Up, Moving_Down,
    Elevation_Change, Became_Idle, Began_Moving
};

#endif //COLDSPACE_ENTITYEVENTS_H
