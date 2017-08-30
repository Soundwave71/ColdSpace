//
// Created by Leonardo paroli on 07/08/2017.
//

#ifndef ENTROPYGUI_MOUSECONTROL_H
#define ENTROPYGUI_MOUSECONTROL_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

using EntityId= unsigned int;
using RouterList= std::unordered_map<EntityId,std::vector<sf::Vector2f>>;
using EntityList = std::vector<EntityId>;

class Behaviour;
class Targeting;
class Pathfinder;
class SystemManager;

class MouseControl {
public:
    MouseControl(): m_mouseCoords(sf::Vector2f(-1,-1)),
                    m_mousePressed(false),
                    m_selection(false),
                    m_releaseCutter(false),
                    m_behaviour(nullptr),
                    m_systemManager(nullptr),
                    m_pathfinder(nullptr),
                    m_renderWindow(nullptr),
                    m_lastPosition(nullptr),
                    m_initialBuffer(true){};

    ~MouseControl();

    void Setup(SystemManager*l_systemManager,
               RouterList*l_routeList,
               Behaviour*l_behaviour,
               Pathfinder* l_pathfinder,
               std::unordered_map<EntityId,std::pair<sf::Vector2f,sf::Vector2f>>* l_lastPosition,
               sf::RenderWindow* l_renderWindow);

    void MouseClick(EntityList l_entities);
    void Move();
    void UnSelect();
    void Patrol();

    bool IsMousePressed(){return m_mousePressed;};
    sf::Vector2f GetMouseCoords(){return m_mouseCoords;};
    bool IsSelected(){return m_selection;};
    std::vector<EntityId>* GetSelectionList(){return &m_selection_list;};

private:
    bool m_mousePressed;
    bool m_selection;
    sf::Vector2f m_mouseCoords;
    std::vector<EntityId> m_selection_list;
    std::unordered_map<EntityId,std::pair<sf::Vector2f,sf::Vector2f>>* m_lastPosition;

    Behaviour*m_behaviour;
    RouterList* m_routerList;
    SystemManager* m_systemManager;
    Pathfinder* m_pathfinder;
    sf::RenderWindow* m_renderWindow;
    bool m_releaseCutter;
    bool m_initialBuffer;

};


#endif //ENTROPYGUI_MOUSECONTROL_H
