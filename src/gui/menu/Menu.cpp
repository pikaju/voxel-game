#include "Menu.h"

Menu::Menu(Game* game) : m_game(game), m_state(MENUSTATE_MAIN), m_mainScreen(this), m_optionsScreen(this)
{
}

Menu::~Menu()
{
}

void Menu::initialize(GUISystem& guiSystem)
{
	m_mainScreen.initialize(guiSystem);
	m_optionsScreen.initialize(guiSystem);
}

void Menu::cleanup()
{
	m_optionsScreen.cleanup();
	m_mainScreen.cleanup();
}

void Menu::update(float delta)
{
	m_mainScreen.setActive(m_state == MENUSTATE_MAIN);
	m_mainScreen.update(delta);
	m_optionsScreen.setActive(m_state == MENUSTATE_OPTIONS);
	m_optionsScreen.update(delta);
}
