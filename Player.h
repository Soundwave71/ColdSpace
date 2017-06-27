//
// Created by Leonardo on 6/27/2017.
//

#ifndef COLDSPACE_PLAYER_H
#define COLDSPACE_PLAYER_H

#include "Character.h"
#include "EventManager.h"

class Player : public Character{
public:
    Player(EntityManager* l_entityMgr);
    ~Player();

    void OnEntityCollision(EntityBase* l_collider, bool l_attack);
    void React(EventDetails* l_details);
};


#endif //COLDSPACE_PLAYER_H
