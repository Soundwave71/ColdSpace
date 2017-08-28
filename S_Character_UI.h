//
// Created by Leonardo paroli on 30/07/2017.
//

#ifndef ENTROPYGUI_S_CHARACTER_UI_H
#define ENTROPYGUI_S_CHARACTER_UI_H

#include "S_Base.h"
#include "Window.h"
#include "C_UI_Element.h"
#include "C_Health.h"

class S_Character_UI : public S_Base{
public:
    S_Character_UI(SystemManager* l_systemMgr);
    ~S_Character_UI();
    void SetUp(TextureManager* l_texture_mgr);
    void Update(float l_dT);
    void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event);
    void Notify(const Message& l_message);

    void Render(Window* l_wind);
private:
    sf::Sprite m_heartBar;
    sf::Vector2u m_heartBarSize;
    TextureManager* m_texturemgr;
};
#endif //ENTROPYGUI_S_CHARACTER_UI_H
