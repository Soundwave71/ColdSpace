#include "S_Renderer.h"
#include "System_Manager.h"
#include "Map.h"

S_Renderer::S_Renderer(SystemManager* l_systemMgr)
	:S_Base(System::Renderer, l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Position);
	req.TurnOnBit((unsigned int)Component::SpriteSheet);
	m_requiredComponents.push_back(req);
	req.Clear();

	m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Direction_Changed,this);
    m_gameMap=nullptr;
    //debug tools
    m_debug=false;
    m_drawLines=false;
}
	
S_Renderer::~S_Renderer(){}

void S_Renderer::Update(float l_dT){
	EntityManager* entities = m_systemManager->GetEntityManager();
	for(auto &entity : m_entities)
	{
		C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
		C_Drawable* drawable = nullptr;
		if (entities->HasComponent(entity, Component::SpriteSheet)){
			drawable = entities->GetComponent<C_Drawable>(entity, Component::SpriteSheet);
		} else { continue; }
		drawable->UpdatePosition(position->GetPosition());
	}
}

void S_Renderer::HandleEvent(const EntityId& l_entity, const EntityEvent& l_event)
{
	if (l_event == EntityEvent::Moving_Left || l_event == EntityEvent::Moving_Right ||
		l_event == EntityEvent::Moving_Up || l_event == EntityEvent::Moving_Down ||
		l_event == EntityEvent::Elevation_Change || l_event == EntityEvent::Spawned)
	{
		SortDrawables();
	}
}

void S_Renderer::Notify(const Message& l_message){
	if(HasEntity(l_message.m_receiver)){
		EntityMessage m = (EntityMessage)l_message.m_type;
		switch(m){
		case EntityMessage::Direction_Changed: 
			SetSheetDirection(l_message.m_receiver, (Direction)l_message.m_int);
			break;
		}
	}
}

void S_Renderer::Render(Window* l_wind, unsigned int l_layer)
{
	EntityManager* entities = m_systemManager->GetEntityManager();
	for(auto &entity : m_entities) {
        C_Position *position = entities->GetComponent<C_Position>(entity, Component::Position);
        if (position->GetElevation() < l_layer) { continue; }
        if (position->GetElevation() > l_layer) { break; }
        C_Drawable *drawable = nullptr;
        if (!entities->HasComponent(entity, Component::SpriteSheet)) { continue; }
        drawable = entities->GetComponent<C_Drawable>(entity, Component::SpriteSheet);
        sf::FloatRect drawableBounds;
        drawableBounds.left = position->GetPosition().x - (drawable->GetSize().x / 2);
        drawableBounds.top = position->GetPosition().y - drawable->GetSize().y;
        drawableBounds.width = drawable->GetSize().x;
        drawableBounds.height = drawable->GetSize().y;
        if (!l_wind->GetViewSpace().intersects(drawableBounds)) { continue; }
        drawable->Draw(l_wind->GetRenderWindow());
        if(m_drawLines) {
            //-DebugTool- Vision: shows lines of sight.
            C_Vision *vision = entities->GetComponent<C_Vision>(entity, Component::Vision);
            C_Controller *control = entities->GetComponent<C_Controller>(entity, Component::Controller);
            S_Vision *visionSystem = m_systemManager->GetSystem<S_Vision>(System::Vision);
            unsigned int controlType = control->GetControlType();
            sf::Vector2f entityPosition = position->GetPosition();
            unsigned int visionRadius = vision->GetVisionRadius();
            if (controlType == 1) {
                for (int i = 0; i < 360; i += 4) {
                    sf::Vertex line[] =
                            {
                                    sf::Vertex(entityPosition),
                                    sf::Vertex(sf::Vector2f(entityPosition.x + (visionSystem->Cos360[i] * visionRadius), entityPosition.y + (visionSystem->Sin360[i] * visionRadius)))
                            };
                    l_wind->GetRenderWindow()->draw(line, 2, sf::Lines);
                }
            }
        }
        if (m_debug) {


            //-DebugTool- Pathfinder: Shows entities's routes. if they have one.
            auto itr = m_systemManager->GetSystem<S_Control>(System::Control)->GetRouterList()->find(entity);
            if (itr != m_systemManager->GetSystem<S_Control>(System::Control)->GetRouterList()->end()) {
                for (int i = 1; i < itr->second.size(); i++) {
                    sf::Vertex line[] =
                            {
                                    sf::Vertex(itr->second[i - 1]),
                                    sf::Vertex(itr->second[i])
                            };
                    line->color = sf::Color::Red;
                    l_wind->GetRenderWindow()->draw(line, 2, sf::Lines);
                }
            }
        }
    }

    ///Drawing selection rectangle if mouse pressed
    MouseControl* mouseControl= m_systemManager->GetSystem<S_Control>(System::Control)->GetMouseControl();
    if (mouseControl->IsMousePressed()) {
        sf::RenderWindow* renderer=l_wind->GetRenderWindow();
        sf::Vector2f m_mouse_start = mouseControl->GetMouseCoords();
        sf::Vector2f m_mouse= renderer->mapPixelToCoords(sf::Mouse::getPosition(*renderer));
        sf::Vertex topline[] =
                {
                        sf::Vertex(m_mouse_start),
                        sf::Vertex(sf::Vector2f(m_mouse.x, m_mouse_start.y))
                };
        sf::Vertex botline[] =
                {
                        sf::Vertex(sf::Vector2f(m_mouse_start.x, m_mouse.y)),
                        sf::Vertex(m_mouse)
                };
        sf::Vertex leftline[] =
                {
                        sf::Vertex(m_mouse_start),
                        sf::Vertex(sf::Vector2f(m_mouse_start.x, m_mouse.y))
                };
        sf::Vertex rightline[] =
                {
                        sf::Vertex(sf::Vector2f(m_mouse.x, m_mouse_start.y)),
                        sf::Vertex(m_mouse)
                };

        topline[0].color = sf::Color::Green;
        topline[1].color = sf::Color::Green;
        botline[0].color = sf::Color::Green;
        botline[1].color = sf::Color::Green;
        leftline[0].color = sf::Color::Green;
        leftline[1].color = sf::Color::Green;
        rightline[0].color = sf::Color::Green;
        rightline[1].color = sf::Color::Green;

        l_wind->GetRenderWindow()->draw(topline, 2, sf::Lines);
        l_wind->GetRenderWindow()->draw(botline, 2, sf::Lines);
        l_wind->GetRenderWindow()->draw(leftline, 2, sf::Lines);
        l_wind->GetRenderWindow()->draw(rightline, 2, sf::Lines);
    }

    //drawing entity selection rect, if selected.
    if(mouseControl->IsSelected()) {
        for (auto entity : *mouseControl->GetSelectionList())
        {
            C_Position *position = entities->GetComponent<C_Position>(entity, Component::Position);
            sf::Vector2f entity_position= position->GetPosition();
            sf::Vertex botline[] =
                    {
                            sf::Vertex(entity_position + sf::Vector2f(-10,10)),
                            sf::Vertex(entity_position+ sf::Vector2f(10,10))
                    };
            sf::Vertex leftline[] =
                    {
                            sf::Vertex(entity_position+ sf::Vector2f(-10,-10)),
                            sf::Vertex(entity_position+ sf::Vector2f(-10,10))
                    };
            sf::Vertex rightline[] =
                    {
                            sf::Vertex(entity_position+ sf::Vector2f(10,-10)),
                            sf::Vertex(entity_position+ sf::Vector2f(10,10))
                    };

            botline[0].color = sf::Color::Green;
            botline[1].color = sf::Color::Green;
            leftline[0].color = sf::Color::Green;
            leftline[1].color = sf::Color::Green;
            rightline[0].color = sf::Color::Green;
            rightline[1].color = sf::Color::Green;

            l_wind->GetRenderWindow()->draw(botline, 2, sf::Lines);
            l_wind->GetRenderWindow()->draw(leftline, 2, sf::Lines);
            l_wind->GetRenderWindow()->draw(rightline, 2, sf::Lines);
        }
    }
}

void S_Renderer::SetSheetDirection(const EntityId& l_entity, const Direction& l_dir)
{
	EntityManager* entities = m_systemManager->GetEntityManager();
	if (!entities->HasComponent(l_entity, Component::SpriteSheet)){ return; }
	C_SpriteSheet* sheet = entities->GetComponent<C_SpriteSheet>(l_entity,Component::SpriteSheet);
	sheet->GetSpriteSheet()->SetDirection(l_dir);
}

void S_Renderer::SortDrawables(){
	EntityManager* e_mgr = m_systemManager->GetEntityManager();
	std::sort(m_entities.begin(), m_entities.end(),
		[e_mgr](unsigned int l_1, unsigned int l_2)
	{
		auto pos1 = e_mgr->GetComponent<C_Position>(l_1, Component::Position);
		auto pos2 = e_mgr->GetComponent<C_Position>(l_2, Component::Position);
		if (pos1->GetElevation() == pos2->GetElevation()){
			return pos1->GetPosition().y < pos2->GetPosition().y;
		}
		return pos1->GetElevation() < pos2->GetElevation();
	});
}