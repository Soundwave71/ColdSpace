#include "S_Control.h"
#include "System_Manager.h"
#include "Map.h"
S_Control::S_Control(SystemManager* l_systemMgr)
	: S_Base(System::Control,l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Position);
	req.TurnOnBit((unsigned int)Component::Movable);
	req.TurnOnBit((unsigned int)Component::Controller);
	m_requiredComponents.push_back(req);
	req.Clear();

    //Subscribing to messages
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Target_Detected,this);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Mouse_Selection, this);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::UnSelect, this);
    m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Selection_Patrol, this);

    // Setting Control Tools
    m_behaviour= new Behaviour();
    m_targeting= new Targeting();

    // Setting MouseControl
    m_mouseControl= new MouseControl();
}

S_Control::~S_Control()
{
    m_behaviour->Purge();
    m_targeting->Purge();
    delete m_behaviour;
    delete m_targeting;
}

void S_Control::Update(float l_dT) {
    C_Position* position;
    for(auto entity:m_entities)
    {
        if(m_behaviour->GetBehaviourList()->find(entity)==m_behaviour->GetBehaviourList()->end()){continue;}
        if (m_behaviour->GetBehaviour(entity) == Behaviours::Guard_Mode)
        {
            if (m_routerList.find(entity) != m_routerList.end())
            {
                m_routerList.erase(entity); //do nothing
            }
        }
        else if(m_behaviour->GetBehaviour(entity) == Behaviours::Move_Mode)
        {
            unsigned int controlType=m_systemManager->GetEntityManager()->GetComponent<C_Controller>(entity, Component::Controller)->GetControlType();
            if(m_routerList.find(entity)==m_routerList.end())
            {
                if(controlType==0)
                {
                    m_routerList.erase(entity);
                    m_behaviour->SetBehaviour(entity, Behaviours::Search_Mode);
                }
                if(controlType==1){
                    m_routerList.erase(entity);
                    m_behaviour->SetBehaviour(entity, Behaviours::Guard_Mode);
                }
            }
        }

        else if (m_behaviour->GetBehaviour(entity) == Behaviours::Attack_Mode)
        {
            //Get target from targetingList, emplace in routerList if no route is found.
            if (m_targeting->HasTarget(entity)) {
                position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity, Component::Position);
                float visionRadius=m_systemManager->GetEntityManager()->GetComponent<C_Vision>(entity, Component::Vision)->GetVisionRadius();
                unsigned int controlType=m_systemManager->GetEntityManager()->GetComponent<C_Controller>(entity, Component::Controller)->GetControlType();
                EntityId targetedEntity = m_targeting->GetTarget(entity);
                sf::Vector2f targetPosition = m_systemManager->GetEntityManager()->GetComponent<C_Position>(targetedEntity, Component::Position)->GetPosition();
                C_Attacker *attacker = m_systemManager->GetEntityManager()->GetComponent<C_Attacker>(entity, Component::Attacker);
                C_Collidable *enemy_collidable = m_systemManager->GetEntityManager()->GetComponent<C_Collidable>(targetedEntity, Component::Collidable);
                EntityState targetState = m_systemManager->GetEntityManager()->GetComponent<C_State>(targetedEntity, Component::State)->GetState();
                sf::Vector2f distance = position->GetPosition()- targetPosition;
                float length = sqrt((distance.x * distance.x) + (distance.y * distance.y));

                //Disengage if target is too far away.
                if(length<visionRadius && targetState!=EntityState::Dying)
                {
                    if(attacker->GetAreaOfAttack().intersects(enemy_collidable->GetCollidable()))
                    {
                        Message attackmsg((MessageType) EntityMessage::Attack);
                        attackmsg.m_receiver = entity;
                        m_systemManager->GetMessageHandler()->Dispatch(attackmsg);
                    }
                    else
                    {
                        m_routerList.erase(entity);
                        m_routerList.emplace(entity, m_pathfinder.Astar(position->GetPosition(), targetPosition));
                        m_pathfinder.ClearRoute();
                    }
                }
                else
                {
                    if(controlType==0)
                    {
                        m_routerList.erase(entity);
                        m_behaviour->SetBehaviour(entity, Behaviours::Search_Mode);
                    }
                    if(controlType==1){
                        m_routerList.erase(entity);
                        m_behaviour->SetBehaviour(entity, Behaviours::Guard_Mode);
                    }
                }
            } else std::cerr << "No target found" << std::endl;
        }

        else if(m_behaviour->GetBehaviour(entity)==Behaviours::Scanning)
        {
            position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity, Component::Position);
            auto timer = m_timers.find(entity);
            if (timer->second > 5.0f)
            {
                //TODO narrow down possible destinations
                sf::Vector2f randomTarget = m_pathfinder.DestinationRandomizer();
                /*std::cout << randomTarget.x << " " << randomTarget.y << std::endl;*/
                m_routerList.erase(entity);
                m_routerList.emplace(entity, m_pathfinder.Astar(position->GetPosition(), randomTarget));
                m_pathfinder.ClearRoute();
                m_behaviour->SetBehaviour(entity, Behaviours::Search_Mode);
                timer->second = 0.0f;
            }
            else
            {
                timer->second += l_dT;
            }
        }

        else if(m_behaviour->GetBehaviour(entity)==Behaviours::Patrol_Mode)
        {
            if( m_routerList.find(entity)==m_routerList.end())
            {
                auto lastPos=m_lastPosition.find(entity);

                //entity entered in patrol mode by error.
                if(lastPos==m_lastPosition.end())
                {
                    std::cerr << "error, patrolling route not found!" << std::endl;
                }
                else
                {
                    sf::Vector2f tempVector;
                    sf::Vector2f entity_position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(entity, Component::Position)->GetPosition();
                    m_routerList.emplace(entity, m_pathfinder.Astar(entity_position, lastPos->second.second));
                    m_pathfinder.ClearRoute();

                    //swap lastpos vectors to create a cycle
                    tempVector=lastPos->second.second;
                    lastPos->second.second=lastPos->second.first;
                    lastPos->second.first=tempVector;
                }
            }
        }

        else if(m_behaviour->GetBehaviour(entity)==Behaviours::Search_Mode)
        {
            //go in Scanning Mode if route cannot be found
            if(m_routerList.find(entity)==m_routerList.end())
            {
               m_behaviour->SetBehaviour(entity, Behaviours::Scanning);
            }
        }
    }
    SortRoutes();
}

void S_Control::HandleEvent(const EntityId& l_entity, 
	const EntityEvent& l_event)
{
    unsigned int control_type=m_systemManager->GetEntityManager()->GetComponent<C_Controller>(l_entity, Component::Controller)->GetControlType();
	if ( control_type== 0) {
		switch (l_event) {

				/////
            case EntityEvent::Spawned:
                if(!SetInLists(l_entity))
                std::cerr << "ERROR, CANNOT SPAWN"<< std::endl;
                m_behaviour->SetBehaviour(l_entity, Behaviours::Search_Mode);

                break;

                /*
            case EntityEvent ::Despawned;
                 if(m_routerList.find(l_entity)!=m_routerList.end())
                 {
                        EraseFromLists(entity);
                 }
                break;
                */
				/////
		}
	} else if (control_type == 1){
		switch (l_event) {

            case EntityEvent::Spawned:
                if(!SetInLists(l_entity))
                    std::cerr << "ERROR, CANNOT SPAWN"<< std::endl;
                m_behaviour->SetBehaviour(l_entity, Behaviours::Guard_Mode);
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

void S_Control::Notify(const Message& l_message){
    EntityManager* eMgr = m_systemManager->GetEntityManager();
    EntityMessage m = (EntityMessage)l_message.m_type;
    sf::RenderWindow* renderer= m_pathfinder.GetMap()->GetContext()->m_wind->GetRenderWindow();
    switch(m)
    {
        case EntityMessage::Target_Detected:

            if(m_behaviour->GetBehaviour((EntityId)l_message.m_sender)==Behaviours::Guard_Mode ||
                    m_behaviour->GetBehaviour((EntityId)l_message.m_sender)==Behaviours::Scanning ||
                    m_behaviour->GetBehaviour((EntityId)l_message.m_sender)==Behaviours::Search_Mode ||
                    m_behaviour->GetBehaviour((EntityId)l_message.m_sender)==Behaviours::Patrol_Mode)
            {
                    m_targeting->SetTarget((EntityId) l_message.m_sender, (EntityId) l_message.m_receiver);
                    m_behaviour->SetBehaviour((EntityId) l_message.m_sender, Behaviours::Attack_Mode);
            }
            break;

        case EntityMessage::Mouse_Selection:
            m_mouseControl->MouseClick(m_entities);
            break;
        case EntityMessage::UnSelect:
            m_mouseControl->UnSelect();
            break;
        case EntityMessage::Selection_Patrol:
            m_mouseControl->Patrol();
    }
}

void S_Control::MoveEntity(const EntityId& l_entity, 
	const Direction& l_dir)
{
	C_Movable* mov = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity, Component::Movable);
	mov->Move(l_dir);
}

bool S_Control::SetInLists(EntityId entity) {
    return !(!m_behaviour->AddEntity(entity)||
             !m_targeting->AddEntity(entity) ||
             !m_timers.emplace(entity, 0.0f).second);
}

bool S_Control::EraseFromLists(EntityId entity) {
    return !(!m_behaviour->RemoveEntity(entity)||
             !m_targeting->RemoveEntity(entity) ||
             !m_timers.erase(entity));
}

void S_Control::SortRoutes() {
    std::pair<sf::Vector2f,bool> target;
    unsigned int ID;
    C_Movable* movable;
    C_Position* position;
    C_State * state;

    for (auto entity = m_routerList.begin(); entity != m_routerList.end(); entity++)
    {
        //setting up
        ID = entity->first;
        movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(ID, Component::Movable);
        position = m_systemManager->GetEntityManager()->GetComponent<C_Position>(ID, Component::Position);
        state=m_systemManager->GetEntityManager()->GetComponent<C_State>(ID, Component::State);
        sf::Vector2f velocity;
        if(state->GetState()==EntityState::Dying || state->GetState()==EntityState::Hurt){continue;}

        //check current position and nodes to reach
        target=m_pathKeeper.CheckRoute(position, &(entity->second));

        if(target.first!=sf::Vector2f(-1000,-1000)) {
            Message msg((MessageType) EntityMessage::Move);
            msg.m_receiver = ID;

            if (target.second) //Last Node -> Arrival().
            {
                    velocity = m_pathKeeper.Arrival(movable, position, target.first);
                    movable->AddVelocity(velocity);
                    if (m_pathKeeper.CollideCheck(movable, position))
                        movable->AddVelocity(m_pathKeeper.Avoidance(movable, position));

                    if (movable->GetVelocity().x > 0) {
                        msg.m_int = (int) Direction::Right;
                        m_systemManager->GetMessageHandler()->Dispatch(msg);
                    } else if (movable->GetVelocity().x < 0) {
                        msg.m_int = (int) Direction::Left;
                        m_systemManager->GetMessageHandler()->Dispatch(msg);
                    } else if (movable->GetVelocity().x == 0 && movable->GetVelocity().y < 0) {
                        msg.m_int = (int) Direction::Up;
                        m_systemManager->GetMessageHandler()->Dispatch(msg);
                    } else if (movable->GetVelocity().x == 0 && movable->GetVelocity().y > 0) {
                        msg.m_int = (int) Direction::Down;
                        m_systemManager->GetMessageHandler()->Dispatch(msg);
                    }
            }
            else //Common Node-> Seek().
            {
                velocity = m_pathKeeper.Seek(movable, position, target.first);
                movable->AddVelocity(velocity);

                if (m_pathKeeper.CollideCheck(movable, position))
                    movable->AddVelocity(m_pathKeeper.Avoidance(movable, position));

                if (movable->GetVelocity().x > 0) {
                    msg.m_int = (int) Direction::Right;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                } else if (movable->GetVelocity().x < 0) {
                    msg.m_int = (int) Direction::Left;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                } else if (movable->GetVelocity().x == 0 && movable->GetVelocity().y < 0) {
                    msg.m_int = (int) Direction::Up;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                } else if (movable->GetVelocity().x == 0 && movable->GetVelocity().y > 0) {
                    msg.m_int = (int) Direction::Down;
                    m_systemManager->GetMessageHandler()->Dispatch(msg);
                }
            }
        }
        else
        {
            m_routerList.erase(entity->first);
        }
    }
}

