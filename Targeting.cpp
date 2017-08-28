//
// Created by Leonardo paroli on 02/08/2017.
//

#include "Targeting.h"

Targeting::Targeting() {}
Targeting::~Targeting()
{
    Purge();
}

void Targeting::Purge()
{
    m_targetingList.clear();
}

void Targeting::SetTarget(EntityId entity, EntityId target) {
    m_targetingList.find(entity)->second=target;
}

EntityId Targeting::GetTarget(EntityId entity) {
    return (EntityId) m_targetingList.find(entity)->second;
}

bool Targeting::HasTarget(EntityId entity) {
    return m_targetingList.find(entity)->second != -1;
}

bool Targeting::AddEntity(EntityId entity, int target) {
    return m_targetingList.emplace(entity, target).second;
}
bool Targeting::RemoveEntity(EntityId entity) {
    return (bool) m_targetingList.erase(entity);
}
