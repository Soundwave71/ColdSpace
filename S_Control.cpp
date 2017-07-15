#include "S_Control.h"
#include "System_Manager.h"

S_Control::S_Control(SystemManager* l_systemMgr)
	: S_Base(System::Control,l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Position);
	req.TurnOnBit((unsigned int)Component::Movable);
	req.TurnOnBit((unsigned int)Component::Controller);
	m_requiredComponents.push_back(req);
	req.Clear();
}

S_Control::~S_Control(){}

void S_Control::Update(float l_dT) {
    std::pair<sf::Vector2f,bool> target;
    unsigned int ID;
    C_Movable* movable;
    C_Position* position;

    for (auto entity = m_routerList.begin(); entity != m_routerList.end(); entity++)
	{
        //setting up
        ID = entity->first;
        movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(ID, Component::Movable);
        position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(ID, Component::Position);

        //check current position and nodes to reach
        target=m_pathKeeper.CheckRoute(position, &(entity->second));

        if(target.first!=sf::Vector2f(-1,-1)) {
            if(target.second) //Last Node -> Arrival().
            {
                movable->AddVelocity(m_pathKeeper.Arrival(movable, position, target.first));
            }
            else //Common Node-> Seek().
            {
                movable->AddVelocity(m_pathKeeper.Seek(movable, position, target.first));
            }
        }
        else //Recalculate destination.
        {
            sf::Vector2f randomTarget= m_pathfinder.DestinationRandomizer();
            entity->second=m_pathfinder.Astar(position->GetPosition(),randomTarget);
        }
	}
}

void S_Control::HandleEvent(const EntityId& l_entity, 
	const EntityEvent& l_event)
{
	if (m_systemManager->GetEntityManager()->GetComponent<C_Controller>(l_entity, Component::Controller)->GetControlType() == 0) {
		switch (l_event) {

				/////
			case EntityEvent::Spawned:
				sf::Vector2f randomTarget= m_pathfinder.DestinationRandomizer();
				m_routerList.emplace(l_entity,m_pathfinder.Astar(m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity,Component::Position)->GetPosition(),randomTarget));
				break;
				/////
		}
	} else if (m_systemManager->GetEntityManager()->GetComponent<C_Controller>(l_entity, Component::Controller)->GetControlType() == 1){
		switch (l_event) {
			case EntityEvent::Moving_Left:
				MoveEntity(l_entity, Direction::Left);
				break;
			case EntityEvent::Moving_Right:
				MoveEntity(l_entity, Direction::Right);
				break;
			case EntityEvent::Moving_Up:
				MoveEntity(l_entity, Direction::Up);
				break;
			case EntityEvent::Moving_Down:
				MoveEntity(l_entity, Direction::Down);
				break;
		}
	}
}

void S_Control::Notify(const Message& l_message){}

void S_Control::MoveEntity(const EntityId& l_entity, 
	const Direction& l_dir)
{
	C_Movable* mov = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity, Component::Movable);
	mov->Move(l_dir);
}