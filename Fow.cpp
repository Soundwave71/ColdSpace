//
// Created by Leonardo on 7/21/2017.
//

#include "Fow.h"
#include "Map.h"

void Fow::BuildFOW(Map* gamemap) {
    m_gamemap = gamemap;

    for (int i = 0; i < GetMapSize().x; i++)
    {
        for (int j = 0; j < GetMapSize().y; j++)
        {
            sf::Vector2u coords = sf::Vector2u( i, j);
            CellID blackID = ConvertCoords(coords.x, coords.y, 4);
            CellID greyID = ConvertCoords(coords.x, coords.y, 3);
            CellID invisibleID = blackID;
            Cell *blackCell = new Cell(true);
            Cell *greyCell = new Cell(true);
            Cell *invisibleCell = new Cell(false);

            blackCell->m_sprite.setTexture(*m_gamemap->GetContext()->m_textureManager->GetResource("BlackCell"));
            greyCell->m_sprite.setTexture(*m_gamemap->GetContext()->m_textureManager->GetResource("GreyCell"));

            m_FOW.emplace(blackID, blackCell);
            m_lingeringFOW.emplace(greyID, greyCell);
            m_enemyFOW.emplace(invisibleID, invisibleCell);
        }
    }
}


unsigned int Fow::ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer) const {
    return ((l_layer*GetMapSize().y+l_y) * GetMapSize().x + l_x);
}

sf::Vector2u Fow::GetMapSize() const
{
    return m_gamemap->GetMapSize();
}

void Fow::ToggleVisibleOn(CellID l_cellID, FowMapList l_mapID) {
    if ((unsigned int) l_mapID == 0) {
        auto itr = m_FOW.find(l_cellID);
        itr->second->Visible();
        if (itr == m_FOW.end())
        {
            std::cerr << "Cell n*"<< l_cellID << " not found!";
            return;
        }
    }
    else if ((unsigned int) l_mapID ==1)
    {
        auto itr = m_lingeringFOW.find(l_cellID);
        itr->second->Visible();
        if (itr == m_lingeringFOW.end())
        {
            std::cerr << "Cell n*"<< l_cellID << " not found!";
            return;
        }
    }
    else
    {
        auto itr = m_enemyFOW.find(l_cellID);
        itr->second->Visible();
        if (itr == m_enemyFOW.end())
        {
            std::cerr << "Cell n*"<< l_cellID << " not found!";
            return;
        }
    }
}

void Fow::ToggleVisibleOff(CellID l_cellID, FowMapList l_mapID) {
    if ((unsigned int) l_mapID == 0) {
        auto itr = m_FOW.find(l_cellID);
        itr->second->Invisible();
        if (itr == m_FOW.end())
        {
            std::cerr << "Cell n*"<< l_cellID << " not found!";
            return;
        }
    }
    else if ((unsigned int) l_mapID ==1)
    {
        auto itr = m_lingeringFOW.find(l_cellID);
        itr->second->Invisible();
        if (itr == m_lingeringFOW.end())
        {
            std::cerr << "Cell n*"<< l_cellID << " not found!";
            return;
        }
    }
    else
    {
        auto itr = m_enemyFOW.find(l_cellID);
        itr->second->Invisible();
        if (itr == m_enemyFOW.end())
        {
            std::cerr << "Cell n*"<< l_cellID << " not found!";
            return;
        }
    }
}

Cell *Fow::GetCell(unsigned int l_x, unsigned int l_y, unsigned int l_layer, FowMapList l_mapID)
{
    if(l_x < 0 || l_y < 0 || l_x >= m_gamemap->GetMapSize().x || l_y >= m_gamemap->GetMapSize().y || l_layer < 0 || l_layer >= Sheet::Num_Layers)
    { return nullptr; }

    if ((unsigned int) l_mapID == 0) {
        auto itr = m_FOW.find(ConvertCoords(l_x,l_y,l_layer));
        if (itr == m_FOW.end())
        {
            /*std::cerr << "Cell at coordinates "<< l_x <<" "<< l_y <<" "<< "at layer "<<" "<< l_layer<<" " << "from map "<<" " << (unsigned int)l_mapID<<" " << " not found!"<<std::endl;*/
            return nullptr;
        }
        return itr->second;
    }
    else if ((unsigned int) l_mapID ==1)
    {
        auto itr = m_lingeringFOW.find(ConvertCoords(l_x,l_y,l_layer));
        if (itr == m_lingeringFOW.end())
        {
            /*std::cerr << "Cell at coordinates "<< l_x <<" "<< l_y <<" "<< "at layer "<<" "<< l_layer<<" " << "from map "<<" " << (unsigned int)l_mapID<<" " << " not found!"<<std::endl;*/
            return nullptr;
        }
        return itr->second;
    }
    else
    {
        auto itr = m_enemyFOW.find(ConvertCoords(l_x,l_y,l_layer));
        if (itr == m_enemyFOW.end())
        {
            /*std::cerr << "Cell at coordinates "<< l_x <<" "<< l_y <<" "<< "at layer "<<" "<< l_layer<<" " << "from map "<<" " << (unsigned int)l_mapID<<" " << " not found!"<<std::endl;*/
            return nullptr;
        }
        return itr->second;
    }
}

void Fow::VisibleFow() {
    for(auto itr: m_FOW)
    {
        itr.second->Visible();
    }
}

void Fow::InvisibleFow()
{
    for(auto itr: m_FOW)
    {
        itr.second->Invisible();
    }
}
