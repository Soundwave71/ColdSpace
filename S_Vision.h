//
// Created by Leonardo on 7/24/2017.
//

#ifndef COLDSPACE_S_VISION_H
#define COLDSPACE_S_VISION_H
#include "S_Base.h"
class Map;

using RadiansAngle=std::array<float,360>;
using TileID= unsigned int;

class S_Vision: public S_Base {
public:
    S_Vision(SystemManager* l_systemMgr);
    ~S_Vision();
    void Update(float l_dt);

    void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event);
    void Notify(const Message& l_message);

    void SetMap(Map* l_gameMap);
    void RayCaster(sf::Vector2f entityPosition, unsigned int visionRadius, unsigned int controlType, EntityId entity);
    std::unordered_map<TileID,sf::Vector2u>BresenhamLine(sf::Vector2f entityPosition, sf::Vector2f lineCooords, int angle, unsigned int visionradius);

    RadiansAngle Cos360;
    RadiansAngle Sin360;

    //Debug tools
    void FOW_OFF();
    void FOW_ON();
    bool m_debug;
private:
    unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const;
    sf::Vector2u ConvertPixelCoords(sf::Vector2f coords) const;

    Map* m_gamemap;
};


#endif //COLDSPACE_S_VISION_H
