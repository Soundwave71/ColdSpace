//
// Created by Leonardo on 7/21/2017.
//

#ifndef COLDSPACE_FOW_H
#define COLDSPACE_FOW_H

#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include "SFML/System/Vector2.hpp"
class Map;

struct Cell {
    Cell(bool needDraw): m_visible(true), m_needDraw(needDraw){};

    void Visible(){m_visible=true;};
    void Invisible(){m_visible=false;}
    bool m_visible;
    bool m_needDraw;
    sf::Sprite m_sprite;
};

using CellID= unsigned int;
using FowMap= std::unordered_map<CellID,Cell*>;
enum class FowMapList{ Ally=0,Lingering, Enemy};



class Fow{
public:

    Fow ():m_gamemap(nullptr){};
    ~Fow(){};

    void BuildFOW(Map* gamemap);
    sf::Vector2u GetMapSize()const;
    void ToggleVisibleOn(CellID l_cellID, FowMapList l_mapID);
    void ToggleVisibleOff(CellID l_cellID, FowMapList l_mapID);
    Cell* GetCell(unsigned int l_x, unsigned int l_y, unsigned int l_layer, FowMapList l_mapID);
    FowMap* GetAllyFow(){return &m_FOW;};
    FowMap* GetLingeringFow(){return &m_lingeringFOW;};
    FowMap* GetEnemyFow(){return &m_enemyFOW;};
    unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const;

    //debug tools
    void VisibleFow();
    void InvisibleFow();
private:


    FowMap m_FOW;
    FowMap m_lingeringFOW;
    FowMap m_enemyFOW;
    Map* m_gamemap;
};



#endif //COLDSPACE_FOW_H
