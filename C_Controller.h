//
// Created by Leonardo on 7/1/2017.
//

#ifndef COLDSPACE_C_CONTROLLER_H
#define COLDSPACE_C_CONTROLLER_H

#include "C_Base.h"

class C_Controller : public C_Base{
public:
    C_Controller() : C_Base(Component::Controller){}
    void ReadIn(std::stringstream& l_stream){}
private:

};

#endif //COLDSPACE_C_CONTROLLER_H
