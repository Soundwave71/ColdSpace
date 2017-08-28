//
// Created by Leonardo paroli on 02/08/2017.
//

#ifndef ENTROPYGUI_BEHAVIOUR_H
#define ENTROPYGUI_BEHAVIOUR_H

#include <unordered_map>
#include "S_Base.h"

enum class Behaviours{
    Guard_Mode=0, Patrol_Mode, Attack_Mode, Search_Mode, Move_Mode, Scanning
};
using EntityId=unsigned int;
using BehaviourList= std::unordered_map<EntityId, Behaviours, EnumClassHash>;

class Behaviour {
public:
    Behaviour();
    ~Behaviour();

    void SetBehaviour(EntityId l_id, Behaviours l_behaviour);
    void DefaultBehaviour(EntityId l_id);
    Behaviours GetBehaviour(EntityId l_id);
    BehaviourList* GetBehaviourList(){return &m_behaviourList;};
    bool AddEntity(EntityId l_id, Behaviours l_behaviour=Behaviours::Guard_Mode);
    bool RemoveEntity(EntityId l_id);
    void Purge();
private:
    BehaviourList m_behaviourList;
};


#endif //ENTROPYGUI_BEHAVIOUR_H
