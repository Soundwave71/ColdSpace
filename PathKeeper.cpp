//
// Created by Leonardo on 7/14/2017.
//

#include <cmath>
#include "PathKeeper.h"

PathKeeper::PathKeeper(): m_slowingRadius(32) {}

PathKeeper::~PathKeeper() {}

sf::Vector2f PathKeeper::Seek(C_Movable *l_movable, C_Position *l_position, sf::Vector2f target) {
    sf::Vector2f steering=target-l_position->GetPosition();
    float length = sqrt((steering.x * steering.x) + (steering.y * steering.y));
    if (length ==0)
    {
        return steering;
    }
    else
    {
        steering.x=(steering.x / length)*l_movable->GetMaxVelocity();
        steering.y=(steering.y / length)*l_movable->GetMaxVelocity();
        return steering;
    }
}

std::pair<sf::Vector2f,bool> PathKeeper::CheckRoute(C_Position* l_position ,EntityRoute* route) {
    sf::Vector2f position=l_position->GetPosition();
    sf::Vector2f newtarget= route->back();
    bool arrival= (route->size()==1);
    sf::Vector2f distance=newtarget-position;
    float length = sqrt((distance.x * distance.x) + (distance.y *distance.y));

    if(length<5)
    {
        if(!arrival)
        {
            route->pop_back();
            newtarget= route->back();
            return std::pair<sf::Vector2f,bool>(newtarget, arrival);
        }
        else
        {
            return std::pair<sf::Vector2f,bool>(newtarget, arrival);
        }
    }
    return std::pair<sf::Vector2f,bool>(newtarget, arrival);
}

sf::Vector2f PathKeeper::Arrival(C_Movable *l_movable, C_Position *l_position, sf::Vector2f target) {
    sf::Vector2f steering=target-l_position->GetPosition();
    float length = sqrt((steering.x * steering.x) + (steering.y * steering.y));
    if (length ==0)
    {
        return steering;
    }
    else if(length < m_slowingRadius)
    {
        steering.x=(steering.x / length)*l_movable->GetMaxVelocity()*(length/m_slowingRadius);
        steering.y=(steering.y / length)*l_movable->GetMaxVelocity()*(length/m_slowingRadius);
        return steering;
    }
    else
    {
        steering.x=(steering.x / length)*l_movable->GetMaxVelocity();
        steering.y=(steering.y / length)*l_movable->GetMaxVelocity();
        return steering;
    }
}


