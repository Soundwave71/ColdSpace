//
// Created by Leonardo on 7/14/2017.
//

#ifndef COLDSPACE_GRID_H
#define COLDSPACE_GRID_H
#include <SFML/System/Vector2.hpp>
#include <algorithm>
class Map;

struct Grid{
    Grid(): m_gamemap(nullptr){};
    ~Grid() {};

    void SetupGrid(Map* gamemap);

    Node* GetNode(sf::Vector2f nodecoords);

    unsigned int ConvertCoordsToID(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const;

    std::vector<Node> m_gridData;
    Map* m_gamemap;
};

#endif //COLDSPACE_GRID_H
