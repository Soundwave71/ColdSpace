//
// Created by Leonardo on 7/3/2017.
//

#ifndef COLDSPACE_GUI_EVENT_H_H
#define COLDSPACE_GUI_EVENT_H_H
#include <string>

enum class GUI_EventType{ None, Click, Release, Hover, Leave };

struct ClickCoordinates{
    float x, y;
};

struct GUI_Event{
    GUI_EventType m_type;
    const char* m_element;
    const char* m_interface;
    union{
        ClickCoordinates m_clickCoords;
    };
};
#endif //COLDSPACE_GUI_EVENT_H_H
