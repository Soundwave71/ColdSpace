//
// Created by Leonardo on 7/11/2017.
//

#ifndef COLDSPACE_PATHFINDER_H
#define COLDSPACE_PATHFINDER_H

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <array>
#include "Node.h"
#include "Grid.h"
class Map;

using Directions= std::array<sf::Vector2f,8>;


class Pathfinder {
public:
    Pathfinder();
    ~Pathfinder();

    void SetMapGrid(Map* gamemap);

    sf::Vector2f DestinationRandomizer();

    std::vector<sf::Vector2f> Astar(sf::Vector2f start, sf::Vector2f target);
    void CheckNeighbours(Node* currentNode, Node* targetNode);
    Map* GetMap() {return m_gamemap;};
    Node*PopBest();
    bool IsOnClosed(Node* node);
    bool IsOnOpen(Node* node);
    void ClearRoute(){route.clear();};

private:
    std::vector<Node*>m_open;
    std::vector<Node*>m_closed;
    Directions m_directions;
    Grid m_grid;
    Map* m_gamemap;
    void ResetGrid();
     sf::Vector2f CenterCoords(sf::Vector2f coords);
    std::vector<sf::Vector2f> route;
};


#endif //COLDSPACE_PATHFINDER_H
