//
// Created by Leonardo paroli on 30/07/2017.
//

#include "S_Character_UI.h"
#include "System_Manager.h"
S_Character_UI::S_Character_UI(SystemManager* l_systemMgr)
        : S_Base(System::Character_UI, l_systemMgr)
{
    Bitmask req;
    req.TurnOnBit((unsigned int)Component::Position);
    req.TurnOnBit((unsigned int)Component::UI_Element);
    req.TurnOnBit((unsigned int)Component::Health);
    m_requiredComponents.push_back(req);
    req.ClearBit((unsigned int)Component::Health);
    m_requiredComponents.push_back(req);
    m_texturemgr= nullptr;
}

S_Character_UI::~S_Character_UI(){
    m_texturemgr->ReleaseResource("HeartBar");
}

void S_Character_UI::Update(float l_dT){}
void S_Character_UI::HandleEvent(const EntityId& l_entity, const EntityEvent& l_event){}
void S_Character_UI::Notify(const Message& l_message){}

void S_Character_UI::Render(Window* l_wind)
{
    EntityManager* entities = m_systemManager->GetEntityManager();
    for (auto &entity : m_entities){
        C_Health* health = entities->GetComponent<C_Health>(entity, Component::Health);
        C_Position* pos = entities->GetComponent<C_Position>(entity, Component::Position);
        C_UI_Element* ui = entities->GetComponent<C_UI_Element>(entity, Component::UI_Element);
        if (health){
            m_heartBar.setTextureRect(sf::IntRect(0, 0, m_heartBarSize.x * health->GetHealth(), m_heartBarSize.y));
            m_heartBar.setOrigin((m_heartBarSize.x * health->GetHealth()) / 2, m_heartBarSize.y);
            m_heartBar.setPosition(pos->GetPosition() + ui->GetOffset());
            l_wind->GetRenderWindow()->draw(m_heartBar);
        }
    }
}

void S_Character_UI::SetUp(TextureManager *l_texture_mgr) {
    l_texture_mgr->RequireResource("HeartBar");
    sf::Texture* txtr = l_texture_mgr->GetResource("HeartBar");
    txtr->setRepeated(true);
    m_heartBarSize = txtr->getSize();
    m_heartBar.setTexture(*txtr);
    m_heartBar.setScale(0.5f, 0.5f);
    m_heartBar.setOrigin(m_heartBarSize.x / 2, m_heartBarSize.y);
    m_texturemgr= l_texture_mgr;
}
