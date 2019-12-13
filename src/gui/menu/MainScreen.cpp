#include "MainScreen.h"

#include "Menu.h"
#include "../GUIScreen.h"
#include "../element/GUILabel.h"
#include "../element/button/GUIButton.h"
#include "../../Game.h"

MainScreen::MainScreen(Menu* menu) : m_menu(menu)
{
}

MainScreen::~MainScreen()
{
}

void MainScreen::initialize(GUISystem& guiSystem)
{
	m_screen = new GUIScreen();
	m_screen->add(new GUILabel("Voxel Game", 100.0f, 100.0f, 12.0f));
	m_screen->add(new GUILabel("Development Build", 100.0f, 200.0f, 4.0f));

	m_play = new GUIButton("Play", 100.0f, 400.0f, 400.0f, 61.0f, 5.0f);
	m_options = new GUIButton("Options", 100.0f, 480.0f, 400.0f, 61.0f, 5.0f);
	m_quit = new GUIButton("Quit", 100.0f, 560.0f, 400.0f, 61.0f, 5.0f);

	m_screen->add(m_play);
	m_screen->add(m_options);
	m_screen->add(m_quit);

	guiSystem.add(m_screen);
}

void MainScreen::cleanup()
{
}

void MainScreen::update(float delta)
{
	if (m_play->wasClicked()) {
		m_menu->getGame()->setState(GAMESTATE_WORLD);
	}
	if (m_options->wasClicked()) {
		m_menu->setState(MENUSTATE_OPTIONS);
	}
}

void MainScreen::setActive(bool active)
{
	m_screen->setActive(active);
}