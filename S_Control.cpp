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
    m_timer=0.0f;
}

S_Control::~S_Control(){}

void S_Control::Update(float l_dT) {
    std::pair<sf::Vector2f,bool> target;
    unsigned int ID;
    C_Movable* movable;
    C_Position* position;
    C_SpriteSheet* sheet;
    C_State * state;

    for (auto entity = m_routerList.begin(); entity != m_routerList.end(); entity++)
	{
        //setting up
        ID = entity->first;
        movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(ID, Component::Movable);
        position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(ID, Component::Position);
        sheet=m_systemManager->GetEntityManager()->GetComponent<C_SpriteSheet>(ID, Component::SpriteSheet);
        sf::Vector2f velocity;
        EntityEvent event;

        //check current position and nodes to reach
        target=m_pathKeeper.CheckRoute(position, &(entity->second));

        if(target.first!=sf::Vector2f(-1000,-1000))
        {
            Message msg((MessageType)EntityMessage::Move);
            msg.m_receiver=ID;

            if(target.second) //Last Node -> Arrival().
            {
                velocity= m_pathKeeper.Arrival(movable, position, target.first);
                movable->AddVelocity(velocity);
                if(m_pathKeeper.CollideCheck(movable,position))
                    movable->AddVelocity(m_pathKeeper.Avoidance(movable, position));

                if(movable->GetVelocity().x>0)
                {
                    msg.m_int = (int) Direction::Right;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
                else if(movable->GetVelocity().x<0)
                {
                    msg.m_int =(int) Direction::Left;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
                else if(movable->GetVelocity().x==0 && movable->GetVelocity().y<0)
                {
                    msg.m_int =(int) Direction::Up;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
                else if(movable->GetVelocity().x==0 && movable->GetVelocity().y>0)
                {
                    msg.m_int =(int) Direction::Down;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }



            }
            else //Common Node-> Seek().
            {
                velocity=m_pathKeeper.Seek(movable, position, target.first);
                movable->AddVelocity(velocity);

               if(m_pathKeeper.CollideCheck(movable,position))
                    movable->AddVelocity(m_pathKeeper.Avoidance(movable, position));

                if(movable->GetVelocity().x>0)
                {
                    msg.m_int = (int) Direction::Right;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
                else if(movable->GetVelocity().x<0)
                {
                    msg.m_int =(int) Direction::Left;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
                else if(movable->GetVelocity().x==0 && movable->GetVelocity().y<0)
                {
                    msg.m_int =(int) Direction::Up;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
                else if(movable->GetVelocity().x==0 && movable->GetVelocity().y>0)
                {
                    msg.m_int =(int) Direction::Down;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }



            }
        }
        else //Recalculate destination.
        {
            if(m_timer > 5.0f)
            {
                sf::Vector2f randomTarget= m_pathfinder.DestinationRandomizer();
                std::cout << randomTarget.x << " " << randomTarget.y << std::endl;
                std::vector<sf::Vector2f> tempvector(m_pathfinder.Astar(position->GetPosition(),randomTarget));
                m_pathfinder.ClearRoute();
                entity->second.swap(tempvector);
                m_timer=0;
            }
            else
            {
                m_timer += l_dT;

            }
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

                    sf::Vector2f randomTarget = m_pathfinder.DestinationRandomizer();
                    std::cout << randomTarget.x << " " << randomTarget.y << std::endl;
                    m_routerList.emplace(l_entity, m_pathfinder.Astar(m_systemManager->GetEntityManager()->GetComponent<C_Position>(l_entity, Component::Position)->GetPosition(), randomTarget));
                    m_pathfinder.ClearRoute();
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