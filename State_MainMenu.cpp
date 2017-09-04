#include "State_MainMenu.h"
#include "StateManager.h"

State_MainMenu::State_MainMenu(StateManager* l_stateManager)
	: BaseState(l_stateManager){}

State_MainMenu::~State_MainMenu(){}

void State_MainMenu::OnCreate(){
	sf::Vector2u windowSize = m_stateMgr->GetContext()
			->m_wind->GetRenderWindow()->getSize();
	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;

	textureMgr->RequireResource("IntroBackground");
	m_background.setTexture(*textureMgr->GetResource("IntroBackground"));
	m_background.setOrigin(textureMgr->GetResource("IntroBackground")->getSize().x / 2.0f,
						   textureMgr->GetResource("IntroBackground")->getSize().y / 2.0f);

	m_background.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	textureMgr->RequireResource("Intro");
	m_introTitle.setTexture(*textureMgr->GetResource("Intro"));
	m_introTitle.setOrigin(textureMgr->GetResource("Intro")->getSize().x / 2.0f,
						   textureMgr->GetResource("Intro")->getSize().y / 2.0f);

	m_introTitle.setPosition(windowSize.x/2.0f,textureMgr->GetResource("Intro")->getSize().y / 2.0f);
	GUI_Manager* gui = m_stateMgr->GetContext()->m_guiManager;
	gui->LoadInterface(StateType::MainMenu, "MainMenu.interface", "MainMenu");
	gui->GetInterface(StateType::MainMenu, "MainMenu")->SetPosition(sf::Vector2f(250.f, 168.f));
	EventManager* eMgr = m_stateMgr->GetContext()->m_eventManager;
	eMgr->AddCallback(StateType::MainMenu, "MainMenu_Play", &State_MainMenu::Play, this);
	eMgr->AddCallback(StateType::MainMenu, "MainMenu_Quit", &State_MainMenu::Quit, this);
}

void State_MainMenu::OnDestroy(){
	m_stateMgr->GetContext()->m_guiManager->RemoveInterface(StateType::MainMenu, "MainMenu");

	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;
	textureMgr->ReleaseResource("IntroBackground");
	textureMgr->ReleaseResource("Intro");

	EventManager* eMgr = m_stateMgr->GetContext()->m_eventManager;
	eMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
	eMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Quit");
}

void State_MainMenu::Activate(){
	auto& play = *m_stateMgr->GetContext()->m_guiManager->
		GetInterface(StateType::MainMenu, "MainMenu")->GetElement("Play");
	if (m_stateMgr->HasState(StateType::Game)){
		// Resume
		play.SetText("Resume");
	} else {
		// Play
		play.SetText("Play");
	}
}

void State_MainMenu::Play(EventDetails* l_details){ m_stateMgr->SwitchTo(StateType::Game); }

void State_MainMenu::Quit(EventDetails* l_details){ m_stateMgr->GetContext()->m_wind->Close(); }

void State_MainMenu::Draw(){
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
	window->draw(m_background);
	window->draw(m_introTitle);
}

void State_MainMenu::Update(const sf::Time& l_dT){}
void State_MainMenu::Deactivate(){}