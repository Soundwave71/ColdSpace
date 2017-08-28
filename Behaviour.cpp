//
// Created by Leonardo paroli on 02/08/2017.
//

#include "Behaviour.h"


Behaviour::Behaviour() {
}

Behaviour::~Behaviour() {
    Purge();
}

void Behaviour::SetBehaviour(EntityId l_id, Behaviours l_behaviour) {
    auto behaviour=m_behaviourList.find(l_id);
    behaviour->second= l_behaviour;
}

void Behaviour::DefaultBehaviour(EntityId l_id) {
    auto behaviour=m_behaviourList.find(l_id);
    behaviour->second=Behaviours::Guard_Mode;
}

Behaviours Behaviour::GetBehaviour(EntityId l_id) {
    auto behaviour=m_behaviourList.find(l_id);
    return behaviour->second;
}

void Behaviour::Purge() {
    m_behaviourList.clear();
}

bool Behaviour::AddEntity(EntityId l_id,Behaviours l_behaviour) {
    return m_behaviourList.emplace(l_id, l_behaviour).second;
}

bool Behaviour::RemoveEntity(EntityId l_id) {
    return (bool) m_behaviourList.erase(l_id);
}
