//
// Created by Leonardo on 7/1/2017.
//

#ifndef COLDSPACE_C_STATE_H
#define COLDSPACE_C_STATE_H

#include "C_Base.h"

enum class EntityState{ Idle, Walking, Attacking, Hurt, Dying };

class C_State : public C_Base{
public:
    C_State(): C_Base(Component::State){}
    void ReadIn(std::stringstream& l_stream){
        unsigned int state = 0;
        l_stream >> state;
        m_state = (EntityState)state;
    }

    EntityState GetState(){ return m_state; }
    void SetState(const EntityState& l_state){
        m_state = l_state;
    }
private:
    EntityState m_state;
};

#endif //COLDSPACE_C_STATE_H
