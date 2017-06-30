//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_OBSERVER_H
#define COLDSPACE_OBSERVER_H

#include "Message.h"

class Observer{
public:
    virtual ~Observer(){}
    virtual void Notify(const Message& l_message) = 0;
};

#endif //COLDSPACE_OBSERVER_H
