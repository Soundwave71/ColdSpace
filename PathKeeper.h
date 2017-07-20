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
    PathKeeper():m_gamemap(nullptr),m_slowingRadius(32), m_avoidanceForce(100){};
    ~PathKeeper();

    sf::Vector2f Seek(C_Movable* l_movable, C_Position* l_position, sf::Vector2f target);
    std::pair<sf::Vector2f,bool> CheckRoute( C_Position* l_position,std::vector<sf::Vector2f>* route);
    sf::Vector2f Arrival(C_Movable* l_movable, C_Position* l_position, sf::Vector2f target);
    sf::Vector2f CalculateAhead (C_Movable* l_movable,C_Position* l_position);
   bool CollideCheck(C_Movable* l_movable,C_Position* l_position );
    sf::Vector2f Avoidance(C_Movable* l_movable,C_Position* l_position);
    void SetPathKeeperMap(Map* gamemap){m_gamemap=gamemap;};
private:
    unsigned int m_slowingRadius;
    float m_avoidanceForce;
    Map* m_gamemap;
};


#endif //COLDSPACE_PATHKEEPER_H
