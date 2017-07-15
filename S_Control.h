#pragma once
#include "S_Base.h"
#include "Pathfinder.h"
#include "PathKeeper.h"

using RouterList= std::unordered_map<EntityId,EntityRoute>;

class S_Control : public S_Base{
public:
	S_Control(SystemManager* l_systemMgr);
	~S_Control();

	void Update(float l_dT);
	void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
	void Notify(const Message& l_message);
	Pathfinder * GetPathfinder(){return &m_pathfinder;};

private:
	void MoveEntity(const EntityId& l_entity, const Direction& l_dir);
	RouterList m_routerList;
	Pathfinder m_pathfinder;
	PathKeeper m_pathKeeper;
};