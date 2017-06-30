//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_C_BASE_H
#define COLDSPACE_C_BASE_H

#include <iostream>
#include <sstream>
#include "ECS_Types.h"

//Base component, base class for all other components classes. ReadIn inputs configurations from external files.

class C_Base{
public:
    C_Base(const Component& l_type): m_type(l_type){}
    virtual ~C_Base(){}

    Component GetType(){ return m_type; }

    friend std::stringstream& operator >>(
            std::stringstream& l_stream, C_Base& b)
    {
        b.ReadIn(l_stream);
        return l_stream;
    }

    virtual void ReadIn(std::stringstream& l_stream) = 0;
protected:
    Component m_type;
};
#endif //COLDSPACE_C_BASE_H
