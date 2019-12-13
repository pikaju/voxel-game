#include "Game.h"

#include "gui/GUIScreen.h"
#include "gui/element/button/GUIButton.h"
#include "gui/element/GUILabel.h"

Game::Game() : m_state(GAMESTATE_MENU), m_world(this), m_guiSystem(), m_menu(this)
{
}

Game::~Game()
{
}

void Game::initialize()
{
	m_world.initialize();
	m_guiSystem.initialize();
	m_menu.initialize(m_guiSystem);
}

void Game::cleanup()
{
	m_menu.cleanup();
	m_guiSystem.cleanup();
	m_world.cleanup();
}

void Game::update(float delta, const vt::Input& input)
{
	switch (m_state) {
	case GAMESTATE_MENU:
		m_menu.update(delta);
		m_guiSystem.update(delta, input);
		break;
	case GAMESTATE_WORLD:
		m_world.update(delta, input);
		if (input.getKeyState(input.KEY_ESCAPE)) {
			m_state = GAMESTATE_MENU;
		}
		break;
	}
}

void Game::render(float delta, const vt::Window* window)
{
	switch (m_state) {
	case GAMESTATE_MENU:
		m_guiSystem.render(window);
		break;
	case GAMESTATE_WORLD:
		m_world.render(delta, window);
		break;
	}
}
