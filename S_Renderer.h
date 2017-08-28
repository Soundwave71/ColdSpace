#pragma once
#include "S_Base.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Window.h"
class Map;
class S_Renderer : public S_Base{
public:
	S_Renderer(SystemManager* l_systemMgr);
	~S_Renderer();

	void Update(float l_dT);
	void HandleEvent(const EntityId& l_entity,const EntityEvent& l_event);
	void Notify(const Message& l_message);
	void Render(Window* l_wind, unsigned int l_layer);
	void SetMap(Map* l_gameMap){m_gameMap= l_gameMap;};

	//debug tools

	bool m_debug;
private:
	void SetSheetDirection(const EntityId& l_entity, const Direction& l_dir);
	void SortDrawables();
	Map* m_gameMap;
};