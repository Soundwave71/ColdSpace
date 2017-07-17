//
// Created by Leonardo on 7/14/2017.
//

#include "Map.h"

void Grid::SetupGrid(Map *gamemap) {
    m_gamemap=gamemap;
    const TileMap* tilemap=gamemap->GetTileMap();
        for(auto itr=tilemap->begin(); itr!= tilemap->end(); itr++)
        {
            Node tempNode(itr->first,itr->second->m_solid);
            m_gridData.emplace_back(tempNode);
        }
}

Node *Grid::GetNode(sf::Vector2f nodecoords) {
    sf::Vector2u IDCoords;
    IDCoords.x = (unsigned int) (nodecoords.x / 32);
    IDCoords.y = (unsigned int) (nodecoords.y / 32);
    unsigned int id = ConvertCoordsToID(IDCoords.x, IDCoords.y, 1);
    for (auto itr = m_gridData.begin(); itr != m_gridData.end(); itr++) {
        if (itr->m_ID == id)
            return &(*itr);
    }
    std::cout << "Unable to find Node:";
    return nullptr;
}

unsigned int Grid::ConvertCoordsToID(unsigned int l_x, unsigned int l_y, unsigned int l_layer) const {
        return ((l_layer*m_gamemap->GetMapSize().y+l_y) * m_gamemap->GetMapSize().x + l_x);
}
