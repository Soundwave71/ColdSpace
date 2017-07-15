//
// Created by Leonardo on 7/14/2017.
//

#ifndef COLDSPACE_NODE_H
#define COLDSPACE_NODE_H

#include <SFML/System/Vector2.hpp>
#include <algorithm>

struct Node{
    Node(unsigned int ID, bool isSolid):
            m_isSolid(isSolid),
            m_ID(ID),
            m_leash(nullptr),
            m_gCost(0),
            m_hCost(0),
            m_fCost(0){};

    unsigned int getCost(){
        return m_fCost;
    }

    bool IsSolid(){return m_isSolid;};

    void ComputeCost(Node* goal, Node* current, bool diagonal){

        if(diagonal)
            m_gCost= current->m_gCost + 14;
        else
            m_gCost=current->m_gCost+ 10;

        unsigned int dx=abs(m_coords.x - goal->m_coords.x);
        unsigned int dy=abs(m_coords.y - goal->m_coords.y);
        m_hCost=10*( dx + dy) - 6 * std::min(dx, dy);
        m_fCost=m_gCost + m_hCost;
    }

    bool IsVisited(){
        return m_visited;
    }

    void Reset(){
        m_gCost=0;
        m_hCost=0;
        m_fCost=0;
        m_visited=false;
        m_coords={-1,-1};
    }

    bool m_visited;
    unsigned int m_ID;
    sf::Vector2f m_coords;
    bool m_isSolid;
    unsigned int m_gCost;
    unsigned int m_hCost;
    unsigned int m_fCost;
    Node* m_leash;
};
#endif //COLDSPACE_NODE_H
