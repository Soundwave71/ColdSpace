//
// Created by Leonardo paroli on 30/07/2017.
//

#ifndef ENTROPYGUI_S_COMBAT_H
#define ENTROPYGUI_S_COMBAT_H

#include "S_Base.h"
#include "C_Position.h"
#include "C_Attacker.h"
#include "C_Health.h"
#include "S_State.h"
#include "C_Movable.h"

class S_Combat : public S_Base{
public:
    S_Combat(SystemManager* l_systemMgr);
    ~S_Combat();

    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event);
    void Notify(const Message& l_message);
private:

};


#endif //ENTROPYGUI_S_COMBAT_H
