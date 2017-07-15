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
using EntityRoute = std::vector<sf::Vector2f>;


class Pathfinder {
public:
    Pathfinder();
    ~Pathfinder();

    void SetMapGrid(Map* gamemap);

    sf::Vector2f DestinationRandomizer();

    EntityRoute Astar(sf::Vector2f start, sf::Vector2f target);
    void CheckNeighbours(Node* currentNode, Node* targetNode);
    Node*PopBest();
    bool IsOnClosed(Node* node);
    bool IsOnOpen(Node* node);

private:
    std::vector<Node*>m_open;
    std::vector<Node*>m_closed;
    Directions m_directions;
    Grid m_grid;
    Map* m_gamemap;
    void ResetGrid();
};


#endif //COLDSPACE_PATHFINDER_H
