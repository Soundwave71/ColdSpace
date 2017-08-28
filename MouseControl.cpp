//
// Created by Leonardo paroli on 07/08/2017.
//
#include "MouseControl.h"
#include "Behaviour.h"
#include "Pathfinder.h"
#include "System_Manager.h"

MouseControl::~MouseControl() {}

void MouseControl::Move() {
    EntityManager *eMgr = m_systemManager->GetEntityManager();
    if (!m_releaseCutter) {
        for (auto entity: m_selection_list)
        {
            sf::Vector2f position = eMgr->GetComponent<C_Position>(entity, Component::Position)->GetPosition();
            m_behaviour->SetBehaviour(entity, Behaviours::Move_Mode);

            auto itr = m_routerList->find(entity);
            if (itr == m_routerList->end())
            {
                m_routerList->emplace(entity, m_pathfinder->Astar(position, m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow))));
            }
            else
            {
                itr->second = m_pathfinder->Astar(position, m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow)));
            }
            m_pathfinder->ClearRoute();
        }
        m_selection_list.clear();
        m_releaseCutter= true;
    }
    else
    {
        m_selection=false;
        m_releaseCutter=false;
    }
}

void MouseControl::Setup(SystemManager *l_systemManager, RouterList *l_routerList, Behaviour *l_behaviour, Pathfinder* l_pathfinder, sf::RenderWindow* l_renderWindow) {
    m_behaviour=l_behaviour;
    m_systemManager=l_systemManager;
    m_routerList=l_routerList;
    m_pathfinder=l_pathfinder;
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
            sf::FloatRect selection_rect(m_mouseCoords, m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow)));

            for (auto entity: l_entities) {
                C_Collidable* entity_collidable = eMgr->GetComponent<C_Collidable>(entity, Component::Collidable);
                unsigned int controlType= eMgr->GetComponent<C_Controller>(entity, Component::Controller)->GetControlType();
                if (selection_rect.intersects(entity_collidable->GetCollidable())&& controlType==1) {
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
