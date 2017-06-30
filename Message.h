//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_MESSAGE_H
#define COLDSPACE_MESSAGE_H

using MessageType = unsigned int;
struct TwoFloats{ float m_x; float m_y; };

struct Message{
    Message(const MessageType& l_type) : m_type(l_type){}

    MessageType m_type;
    int m_sender;
    int m_receiver;

    union{
        TwoFloats m_2f;
        bool m_bool;
        int m_int;
    };
};
#endif //COLDSPACE_MESSAGE_H
