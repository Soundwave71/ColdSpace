//
// Created by Leonardo on 7/24/2017.
//
#include "System_Manager.h"
#include "Map.h"

S_Vision::S_Vision(SystemManager *l_systemMgr) : S_Base(System::Vision, l_systemMgr){

    Bitmask req;
    req.TurnOnBit((unsigned int)Component::Vision);
    req.TurnOnBit((unsigned int)Component::Position);
    req.TurnOnBit((unsigned int)Component::Controller);
    m_requiredComponents.push_back(req);
    req.Clear();

    //pre-computes degrees-to-radians conversions, 360° wide.
    for(int i=0; i<360; i++)
    {
        Sin360[i]=sinf(i*0.01745f);
        Cos360[i]=cosf(i*0.01745f);
    }

    m_gamemap=nullptr;
    m_debug=false;
}

S_Vision::~S_Vision() {}

void S_Vision::Update(float l_dt) {
    if (!m_gamemap){ return; }
    EntityManager* entities = m_systemManager->GetEntityManager();
    Fow* fow= m_gamemap->GetFow();

    //resets Lingering
    for(auto itr :*fow->GetLingeringFow())
    {
        itr.second->Visible();
    }

    for(auto entity: m_entities)
    {
        //TODO : remove performance issues by not checking for enemy fow, but just distance.
        C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
        C_Vision* vision = entities->GetComponent<C_Vision>(entity, Component::Vision);
        C_Controller* control = entities->GetComponent<C_Controller>(entity, Component::Controller);
        RayCaster(position->GetPosition(), vision->GetVisionRadius(),control->GetControlType(), entity);
    }
}

void S_Vision::HandleEvent(const EntityId &l_entity, const EntityEvent &l_event) {}

void S_Vision::Notify(const Message &l_message) {}

void S_Vision::SetMap(Map *l_gameMap) {
    m_gamemap=l_gameMap;
}

void S_Vision::RayCaster(sf::Vector2f entityPosition, unsigned int visionRadius, unsigned int controlType, EntityId entity) {

    //RAYCASTING BEGINS
    Fow *fow = m_gamemap->GetFow();
    unsigned int x;
    unsigned int y;
    sf::Vector2f lineCoords;
    std::unordered_map<TileID, sf::Vector2u> linePoints;
    std::vector<EntityId> targets;

    for (int i = 0; i < 360; i+=4)
    {
        lineCoords = sf::Vector2f(entityPosition.x + (Cos360[i] * visionRadius),
                                  entityPosition.y + (Sin360[i] * visionRadius));
        if(lineCoords.x < 0){lineCoords.x=0.0f;}
        if(lineCoords.y < 0){lineCoords.y=0.0f;}
        if( lineCoords.x >= m_gamemap->GetMapSize().x*Sheet::Tile_Size){lineCoords.x=m_gamemap->GetMapSize().x*Sheet::Tile_Size-1;}
        if( lineCoords.y >= m_gamemap->GetMapSize().y*Sheet::Tile_Size){lineCoords.y=m_gamemap->GetMapSize().y*Sheet::Tile_Size-1;}

        linePoints = BresenhamLine(entityPosition, lineCoords, i, visionRadius);

        //checks every point of the Line.
        for (auto &point: linePoints)
        {
            x=point.second.x;
            y=point.second.y;

            //turning down the fog of war where necessary, along the Line.
            if (fow->GetCell(x, y, 4, FowMapList::Ally)->m_visible && controlType==1)
            {
                fow->ToggleVisibleOff((CellID) fow->ConvertCoords(x, y, 4), FowMapList::Ally);
            }

             if (fow->GetCell(x, y, 3, FowMapList::Lingering)->m_visible &&controlType==1)
            {
                 fow->ToggleVisibleOff((CellID) fow->ConvertCoords(x, y, 3), FowMapList::Lingering);
            }
            /*
             if (controlType == 0 && fow->GetCell(x, y, 4, FowMapList::Enemy)->m_visible)
            {
                 fow->ToggleVisibleOff((CellID) fow->ConvertCoords(x, y, 4), FowMapList::Enemy);
            }
            */
            //check every entity position for intersections with the Line.
            for(auto pos: m_entities) {
                sf::Vector2f pixelcoords = m_systemManager->GetEntityManager()->GetComponent<C_Position>(pos, Component::Position)->GetPosition();
                sf::Vector2u coords = ConvertPixelCoords(pixelcoords);
                unsigned int targetControlType = m_systemManager->GetEntityManager()->GetComponent<C_Controller>(pos, Component::Controller)->GetControlType();

                if (targetControlType != controlType) {
                    if (point.second == coords) {
                        targets.push_back(pos);
                    }
                    if (point.second == coords) {
                        targets.push_back(pos);
                    }
                }
            }
        }
        linePoints.clear();
    }
    //RAYCASTING ENDS

    //if target found
    EntityId nearest;
    float distance = visionRadius + 1;
    if (!targets.empty())
    {
        for (auto itr: targets)
        {

            sf::Vector2f targetPosition = m_systemManager->GetEntityManager()->GetComponent<C_Position>(itr, Component::Position)->GetPosition();
            targetPosition=entityPosition-targetPosition;
            float tempDistance=(float)sqrt((targetPosition.x * targetPosition.x) + (targetPosition.y * targetPosition.y));
            if (tempDistance < distance)
            {
                nearest=itr;
                distance=tempDistance;
            }
        }
        if (distance <= visionRadius)
        {
            Message msg((MessageType) EntityMessage::Target_Detected);
            msg.m_sender =entity;
            msg.m_receiver = nearest;
            m_systemManager->GetMessageHandler()->Dispatch(msg);
        }
    }
}

std::unordered_map<TileID,sf::Vector2u> S_Vision::BresenhamLine(sf::Vector2f entityPosition, sf::Vector2f lineCooords, int angle, unsigned int visionRadius) {

    std::unordered_map<TileID, sf::Vector2u> line;
    sf::Vector2f currentPoint;
    float x1 = entityPosition.x;
    float x2 = lineCooords.x;
    float y1 = entityPosition.y;
    float y2 = lineCooords.y;
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));

    if (steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? Sheet::Tile_Size : -Sheet::Tile_Size;
        int y = (int) y1;

        const float maxX = x2;
        for (float x = x1; x >= maxX; x -= Sheet::Tile_Size)
        {
            if (steep) { currentPoint = sf::Vector2f(y, x); }
            else       { currentPoint = sf::Vector2f(x, y); }
            if (!m_gamemap->GetTile(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1)->m_solid) {
                //check if it's already emplaced
                auto itr = line.find(
                        ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1));
                //if not, emplace it.
                if (itr == line.end()) {
                    line.emplace(
                            ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1),
                            ConvertPixelCoords(currentPoint));
                }
            }

            else
            {   //check if it's already emplaced
                auto itr = line.find(
                        ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1));
                //if not, emplace it.
                if (itr == line.end()) {
                    line.emplace(
                            ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1),
                            ConvertPixelCoords(currentPoint));
                    return line;
                }
            }

            error += dy;
            if (error > 0)
            {
                y += ystep;
                error -= dx;
            }
        }
    }
    else {
        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? Sheet::Tile_Size : -Sheet::Tile_Size;
        int y = (int) y1;

        const float maxX = x2;

        for (float x = x1; x <= maxX; x += Sheet::Tile_Size) {
            if (steep) { currentPoint = sf::Vector2f(y, x); }
            else { currentPoint = sf::Vector2f(x, y); }

            if (!m_gamemap->GetTile(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y,
                                    1)->m_solid) {
                //check if it's already emplaced
                auto itr = line.find(
                        ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1));
                //if not, emplace it.
                if (itr == line.end()) {
                    line.emplace(
                            ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1),
                            ConvertPixelCoords(currentPoint));
                }
            } else {   //check if it's already emplaced
                auto itr = line.find(
                        ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1));
                //if not, emplace it.
                if (itr == line.end()) {
                    line.emplace(
                            ConvertCoords(ConvertPixelCoords(currentPoint).x, ConvertPixelCoords(currentPoint).y, 1),
                            ConvertPixelCoords(currentPoint));
                    return line;
                }
            }

            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }
    return line;
}

sf::Vector2u S_Vision::ConvertPixelCoords(sf::Vector2f coords) const {
    return sf::Vector2u((unsigned int)(coords.x/32),(unsigned int)(coords.y/32));
}

unsigned int S_Vision::ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer) const {
    return ((l_layer*m_gamemap->GetMapSize().y+l_y) *m_gamemap->GetMapSize().x + l_x);
}

void S_Vision::FOW_OFF() {
    Fow* fow=m_gamemap->GetFow();
    fow->InvisibleFow();
}

void S_Vision::FOW_ON() {
    Fow* fow= m_gamemap->GetFow();
    fow->VisibleFow();
}

