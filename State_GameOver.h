//
// Created by Leonardo on 6/27/2017.
//

#ifndef COLDSPACE_STATE_GAMEOVER_H
#define COLDSPACE_STATE_GAMEOVER_H

#include "BaseState.h"
#include <SFML/Graphics.hpp>

class State_GameOver : public BaseState{
public:
    State_GameOver(StateManager* l_stateManager);
    ~State_GameOver();

    void OnCreate();
    void OnDestroy();

    void Activate();
    void Deactivate();

    void Update(const sf::Time& l_time);
    void Draw();
private:
    sf::Font m_font;
    sf::Text m_text;
    float m_elapsed;
};

#endif //COLDSPACE_STATE_GAMEOVER_H
