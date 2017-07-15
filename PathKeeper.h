//
// Created by Leonardo on 7/14/2017.
//

#ifndef COLDSPACE_PATHKEEPER_H
#define COLDSPACE_PATHKEEPER_H

#include "C_Movable.h"
#include "SFML/System/Vector2.hpp"
#include "C_Position.h"
#include "Pathfinder.h"


class PathKeeper {
public:
    PathKeeper();
    ~PathKeeper();

    sf::Vector2f Seek(C_Movable* l_movable, C_Position* l_position, sf::Vector2f target);
    std::pair<sf::Vector2f,bool> CheckRoute( C_Position* l_position,EntityRoute* route);
    sf::Vector2f Arrival(C_Movable* l_movable, C_Position* l_position, sf::Vector2f target);
private:
    unsigned int m_slowingRadius;
};


#endif //COLDSPACE_PATHKEEPER_H
