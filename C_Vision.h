//
// Created by Leonardo on 7/24/2017.
//

#ifndef COLDSPACE_C_VISION_H
#define COLDSPACE_C_VISION_H
#include "C_Base.h"

class C_Vision : public C_Base{
public:
    C_Vision() : C_Base(Component::Controller){}
    void ReadIn(std::stringstream& l_stream){
        l_stream >> m_visionRadius;
    }
    unsigned int GetVisionRadius(){return m_visionRadius;};
private:
    unsigned int m_visionRadius;
};
#endif //COLDSPACE_C_VISION_H
