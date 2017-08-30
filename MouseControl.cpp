//
// Created by Leonardo paroli on 07/08/2017.
//
#include "MouseControl.h"
#include "Behaviour.h"
#include "Pathfinder.h"
#include "System_Manager.h"
#include "Map.h"
MouseControl::~MouseControl() {}

void MouseControl::Move() {
    EntityManager *eMgr = m_systemManager->GetEntityManager();
    if (!m_releaseCutter)
    {

        sf::Vector2f destination = m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));
        unsigned int tileSize = m_pathfinder->GetMap()->GetTileSize();
        sf::Vector2u normalizedCoords = sf::Vector2u((unsigned int) destination.x / tileSize,
                                                     (unsigned int) destination.y / tileSize);
        if (!m_pathfinder->GetMap()->GetTile(normalizedCoords.x, normalizedCoords.y, 1)->m_solid)
        {
            for (auto entity: m_selection_list) {
                sf::Vector2f position = eMgr->GetComponent<C_Position>(entity, Component::Position)->GetPosition();
                m_behaviour->SetBehaviour(entity, Behaviours::Move_Mode);

                auto itr = m_routerList->find(entity);
                if (itr == m_routerList->end()) {
                    m_routerList->emplace(entity, m_pathfinder->Astar(position, destination));
                } else {
                    itr->second = m_pathfinder->Astar(position, destination);
                }
                m_pathfinder->ClearRoute();
            }
            m_releaseCutter = true;
        }
    }
    else
    {
        m_releaseCutter=false;
    }
}

void MouseControl::Setup(SystemManager *l_systemManager,
                         RouterList *l_routeList,
                         Behaviour *l_behaviour,
                         Pathfinder* l_pathfinder,
                         std::unordered_map<EntityId,std::pair<sf::Vector2f,sf::Vector2f>>* l_lastPosition,
                         sf::RenderWindow* l_renderWindow) {
    m_behaviour=l_behaviour;
    m_systemManager=l_systemManager;
    m_routerList=l_routeList;
    m_pathfinder=l_pathfinder;
    m_lastPosition=l_lastPosition;
    m_renderWindow=l_renderWindow;
}

void MouseControl::MouseClick(EntityList l_entities) {

    EntityManager *eMgr = m_systemManager->GetEntityManager();

    //avoids menu-click-release to be buffered.
    if(m_initialBuffer)
    {
        m_initialBuffer = false;
        return;
    }

    if (!m_selection)
    {

        if (!m_mousePressed)
        {
            m_mousePressed = true;
            m_mouseCoords = m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));
        }

        else
        {
            sf::Vector2f newMouseCoords=m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));

            float temp=0;
            sf::Vector2f tempVector(0,0);
            if(newMouseCoords.x>m_mouseCoords.x && newMouseCoords.y<m_mouseCoords.y)
            {
                temp=m_mouseCoords.y;
                m_mouseCoords.y= newMouseCoords.y;
                newMouseCoords.y=temp;
            }
            else if(newMouseCoords.x<m_mouseCoords.x && newMouseCoords.y<m_mouseCoords.y)
            {
                tempVector=newMouseCoords;
                newMouseCoords=m_mouseCoords;
                m_mouseCoords=tempVector;
            }
            else if(newMouseCoords.x<m_mouseCoords.x && newMouseCoords.y>m_mouseCoords.y)
            {
                temp=newMouseCoords.x;
                newMouseCoords.x=m_mouseCoords.x;
                m_mouseCoords.x=temp;
            }
            sf::FloatRect selection_rect(m_mouseCoords,newMouseCoords);
            for (auto entity: l_entities) {
                C_Position* entity_position = eMgr->GetComponent<C_Position>(entity, Component::Position);
                unsigned int controlType= eMgr->GetComponent<C_Controller>(entity, Component::Controller)->GetControlType();
                if (selection_rect.contains(entity_position->GetPosition()) && controlType==1 &&entity_position->GetPosition().x< newMouseCoords.x) {
                    m_selection_list.push_back(entity);
                }
            }

            if (!m_selection_list.empty()) {
                m_selection = true;
            }
            m_mousePressed = false;
            m_mouseCoords = sf::Vector2f(-1, -1);
        }
    }
    else
    {
        Move();
    }
}

void MouseControl::UnSelect() {
    m_selection_list.clear();
    m_selection=false;
}

void MouseControl::Patrol() {
    if (m_selection) {
        EntityManager *eMgr = m_systemManager->GetEntityManager();
        sf::Vector2f destination = m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));
        unsigned int tileSize = m_pathfinder->GetMap()->GetTileSize();
        sf::Vector2u normalizedCoords = sf::Vector2u((unsigned int) destination.x / tileSize,
                                                     (unsigned int) destination.y / tileSize);
        if (!m_pathfinder->GetMap()->GetTile(normalizedCoords.x, normalizedCoords.y, 1)->m_solid) {
            for (auto entity: m_selection_list)
            {
                sf::Vector2f position = eMgr->GetComponent<C_Position>(entity, Component::Position)->GetPosition();
                m_behaviour->SetBehaviour(entity, Behaviours::Patrol_Mode);

                //if a previous iteration emplaced the entity, erase and reset it's placeholder.
                if (m_lastPosition->find(entity) != m_lastPosition->end()) {
                    m_lastPosition->erase(entity);
                }

                //emplaces entity's last position for further use.
                m_lastPosition->emplace(entity, std::pair<sf::Vector2f, sf::Vector2f>(position, destination));
                auto itr = m_routerList->find(entity);
                if (itr == m_routerList->end())
                {
                    m_routerList->emplace(entity, m_pathfinder->Astar(position, destination));
                }
                else
                {
                    itr->second = m_pathfinder->Astar(position, destination);
                }
                m_pathfinder->ClearRoute();
            }
        }
    }
}

