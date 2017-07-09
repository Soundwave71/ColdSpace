//
// Created by Leonardo on 7/9/2017.
//

#ifndef COLDSPACE_C_SOUNDLISTENER_H
#define COLDSPACE_C_SOUNDLISTENER_H
#include "C_Base.h"

class C_SoundListener : public C_Base{
public:
    C_SoundListener() : C_Base(Component::SoundListener){}
    void ReadIn(std::stringstream& l_stream){}
private:

};
#endif //COLDSPACE_C_SOUNDLISTENER_H
