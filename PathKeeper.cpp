//
// Created by Leonardo on 7/14/2017.
//

#include <cmath>
#include "PathKeeper.h"
#include "Map.h"
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
        if(std::abs(steering.x) > l_movable->GetMaxVelocity()/2){
            steering.x = (l_movable->GetMaxVelocity()/2) * (steering.x / std::abs(steering.x));
        }

        if(std::abs(steering.y) > l_movable->GetMaxVelocity()/2){
            steering.y =(l_movable->GetMaxVelocity()/2) * (steering.y / std::abs(steering.y));
        }
        return steering;
    }
}

std::pair<sf::Vector2f,bool> PathKeeper::CheckRoute(C_Position* l_position ,std::vector<sf::Vector2f>* route) {

    if(route->empty())
    {
        return std::pair<sf::Vector2f, bool>(sf::Vector2f(-1000, -1000), false);
    }
    else
    {
        sf::Vector2f position = l_position->GetPosition();
        sf::Vector2f newtarget = route->back();
        bool arrival = (route->size() == 1);
        sf::Vector2f distance = newtarget - position;
        float length = sqrt((distance.x * distance.x) + (distance.y * distance.y));

        if (length < 20)
        {
            if (!arrival)
            {
                route->pop_back();
                newtarget = route->back();
                return std::pair<sf::Vector2f, bool>(newtarget, arrival);
            }
            else
            {
                if(length<15)
                {
                    route->pop_back();
                }
                return std::pair<sf::Vector2f, bool>(newtarget, arrival);
            }
        }
        return std::pair<sf::Vector2f, bool>(newtarget, arrival);
    }
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


bool PathKeeper::CollideCheck(C_Movable *l_movable,C_Position* l_position) {
    sf::Vector2f ahead=CalculateAhead(l_movable, l_position);
    sf::Vector2u aheadCoords= sf::Vector2u(ahead.x/m_gamemap->GetTileSize(), ahead.y/m_gamemap->GetTileSize());
    return m_gamemap->GetTile(aheadCoords.x,aheadCoords.y,1)->m_solid;
}

sf::Vector2f PathKeeper::Avoidance(C_Movable *l_movable, C_Position *l_position) {
    sf::Vector2f ahead=CalculateAhead(l_movable,l_position);
    sf::Vector2u objectCoords= sf::Vector2u(ahead.x/m_gamemap->GetTileSize(), ahead.y/m_gamemap->GetTileSize());
    objectCoords=sf::Vector2u(objectCoords.x*32, objectCoords.y*32)+sf::Vector2u(16,16);
    sf::Vector2f avoidance=ahead-sf::Vector2f(objectCoords);
    avoidance= (avoidance/(float)(sqrt((avoidance.x*avoidance.x)+(avoidance.y*avoidance.y))))*m_avoidanceForce;
    return avoidance;
}

sf::Vector2f PathKeeper::CalculateAhead(C_Movable *l_movable, C_Position *l_position) {
    float velocity_length=sqrt(l_movable->GetVelocity().x *l_movable->GetVelocity().x)+(l_movable->GetVelocity().y *l_movable->GetVelocity().y );
    float dynamic_length=velocity_length/l_movable->GetMaxVelocity();
    sf::Vector2f normalized_velocity=l_movable->GetVelocity()/velocity_length;
    sf::Vector2f ahead=l_position->GetPosition()+ normalized_velocity*dynamic_length;;
    return ahead;
}


sf::Vector2u PathKeeper::ConvertPixelCoords(sf::Vector2f coords) const {
    return sf::Vector2u((unsigned int)(coords.x/32),(unsigned int)(coords.y/32));
}

unsigned int PathKeeper::ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer) const {
    return ((l_layer*m_gamemap->GetMapSize().y+l_y) *m_gamemap->GetMapSize().x + l_x);
}
