//
// Created by Leonardo on 6/26/2017.
//

#ifndef COLDSPACE_ANIM_DIRECTIONAL_H
#define COLDSPACE_ANIM_DIRECTIONAL_H

#include "Anim_Base.h"
#include "Direction.h"

class Anim_Directional : public Anim_Base{
protected:
    void FrameStep();
    void CropSprite();
    void ReadIn(std::stringstream& l_stream);
};



#endif //COLDSPACE_ANIM_DIRECTIONAL_H
