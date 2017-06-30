//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_MESSAGEHANDLER_H
#define COLDSPACE_MESSAGEHANDLER_H

#include "Communicator.h"
#include "EntityMessages.h"
#include "EnumHasher.h"
#include <unordered_map>

using Subscribtions = std::unordered_map<EntityMessage,Communicator, EnumClassHash>;

class MessageHandler{
public:
    bool Subscribe(const EntityMessage& l_type, Observer* l_observer)
    {
        return m_communicators[l_type].AddObserver(l_observer);
    }

    bool Unsubscribe(const EntityMessage& l_type, Observer* l_observer)
    {
        return m_communicators[l_type].RemoveObserver(l_observer);
    }

    void Dispatch(const Message& l_msg){
        auto itr = m_communicators.find((EntityMessage)l_msg.m_type);
        if (itr == m_communicators.end()){ return; }
        itr->second.Broadcast(l_msg);
    }
private:
    Subscribtions m_communicators;
};
#endif //COLDSPACE_MESSAGEHANDLER_H
