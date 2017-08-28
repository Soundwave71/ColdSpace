//
// Created by Leonardo paroli on 02/08/2017.
//

#ifndef ENTROPYGUI_TARGETING_H
#define ENTROPYGUI_TARGETING_H

#include <unordered_map>
#include "S_Base.h"

using EntityId= unsigned int;
using TargetingList =std::unordered_map<EntityId,int>;

class Targeting {
public:
    Targeting();
    ~Targeting();
    void SetTarget(EntityId entity, EntityId target);
    EntityId GetTarget(EntityId entity);
    bool HasTarget(EntityId entity);
    TargetingList* GetTargetingList(){return &m_targetingList;};
    bool AddEntity(EntityId entity, int target=-1);
    bool RemoveEntity(EntityId entity);
    void Purge();
private:
    TargetingList m_targetingList;
};


#endif //ENTROPYGUI_TARGETING_H
