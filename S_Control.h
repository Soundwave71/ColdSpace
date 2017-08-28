#pragma once
#include "S_Base.h"
#include "Pathfinder.h"
#include "PathKeeper.h"
#include "Behaviour.h"
#include "Targeting.h"
#include "MouseControl.h"
using RouterList= std::unordered_map<EntityId,std::vector<sf::Vector2f>>;

class S_Control : public S_Base{
public:
	S_Control(SystemManager* l_systemMgr);
	~S_Control();

	void Update(float l_dT);
	void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
	void Notify(const Message& l_message);
	Pathfinder * GetPathfinder(){return &m_pathfinder;};
	PathKeeper* GetPathKeeper(){return &m_pathKeeper;};
	RouterList* GetRouterList(){return &m_routerList;};
    MouseControl* GetMouseControl(){return m_mouseControl;};
    Behaviour* GetBehaviour(){return m_behaviour;};
	bool SetInLists(EntityId entity);
	bool EraseFromLists(EntityId entity);
	void SortRoutes();

private:
	void MoveEntity(const EntityId& l_entity, const Direction& l_dir);
	Pathfinder m_pathfinder;
	PathKeeper m_pathKeeper;

    Behaviour* m_behaviour;
    Targeting* m_targeting;
    MouseControl* m_mouseControl;

	RouterList m_routerList;
	std::unordered_map<EntityId, float> m_timers;

};