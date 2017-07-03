#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "System_Manager.h"
#include "Entity_Manager.h"
#include "DebugOverlay.h"
#include "FontManager.h"
#include "GUI_Manager.h"

class Map;

struct SharedContext{
	SharedContext():
			m_wind(nullptr),
			m_eventManager(nullptr),
			m_fontManager(nullptr),
			m_guiManager(nullptr),
			m_textureManager(nullptr),
			m_systemManager(nullptr),
			m_entityManager(nullptr),
			m_gameMap(nullptr){}

	Window* m_wind;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
	SystemManager* m_systemManager;
	Entity_Manager* m_entityManager;
	FontManager* m_fontManager;
	GUI_Manager* m_guiManager;
	Map* m_gameMap;
	DebugOverlay m_debugOverlay;
};