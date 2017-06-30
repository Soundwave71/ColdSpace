//
// Created by Leonardo on 6/29/2017.
//

#ifndef COLDSPACE_C_POSITION_H
#define COLDSPACE_C_POSITION_H
#include <SFML/System/Vector2.hpp>
#include "C_Base.h"

//keeps tracks of the current position, old position,  and elevation for rendering purpose.

class C_Position : public C_Base{
public:
    C_Position(): C_Base(Component::Position), m_elevation(0){}
    ~C_Position(){}

    //only reads its specific data.
    void ReadIn(std::stringstream& l_stream){
        l_stream >> m_position.x >> m_position.y >> m_elevation;
    }

    const sf::Vector2f& GetPosition(){ return m_position; }
    const sf::Vector2f& GetOldPosition(){ return m_positionOld; }
    unsigned int GetElevation(){ return m_elevation; }

    void SetPosition(float l_x, float l_y){
        m_positionOld = m_position;
        m_position = sf::Vector2f(l_x,l_y);
    }

    void SetPosition(const sf::Vector2f& l_vec){
        m_positionOld = m_position;
        m_position = l_vec;
    }

    void SetElevation(unsigned int l_elevation){
        m_elevation = l_elevation;
    }

    void MoveBy(float l_x, float l_y){
        m_positionOld = m_position;
        m_position += sf::Vector2f(l_x,l_y);
    }

    void MoveBy(const sf::Vector2f& l_vec){
        m_positionOld = m_position;
        m_position += l_vec;
    }

private:
    sf::Vector2f m_position;
    sf::Vector2f m_positionOld;
    unsigned int m_elevation;
};
#endif //COLDSPACE_C_POSITION_H
