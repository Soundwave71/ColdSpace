//
// Created by Leonardo paroli on 31/07/2017.
//

#ifndef ENTROPYGUI_S_TIMERS_H
#define ENTROPYGUI_S_TIMERS_H

#include "S_Base.h"
#include "C_Attacker.h"
#include "C_Health.h"
#include "S_State.h"

class S_Timers : public S_Base{
public:
    S_Timers(SystemManager* l_systemMgr);
    ~S_Timers();

    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event);
    void Notify(const Message& l_message);
private:
    std::unordered_map<EntityId, sf::Vector2f> m_spawnPoints;
};
#endif //ENTROPYGUI_S_TIMERS_H
