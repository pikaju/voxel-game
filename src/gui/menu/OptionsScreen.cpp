#include "OptionsScreen.h"

#include "Menu.h"
#include "../GUIScreen.h"
#include "../element/GUILabel.h"
#include "../element/button/GUIButton.h"

OptionsScreen::OptionsScreen(Menu* menu) : m_menu(menu)
{
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::initialize(GUISystem& guiSystem)
{
	m_screen = new GUIScreen();
	m_screen->add(new GUILabel("Options", 50.0f, 50.0f, 10.0f));
	
	m_screen->add(new GUIButton("Motion blur", 100.0f, 300.0f, 400.0f, 61.0f, 5.0f));
	m_screen->add(new GUIButton("Resolution", 100.0f, 380.0f, 400.0f, 61.0f, 5.0f));
	m_screen->add(new GUIButton("VSync", 100.0f, 460.0f, 400.0f, 61.0f, 5.0f));
	
	m_cancel = new GUIButton("Cancel", 50.0f, 600.0f, 200.0f, 61.0f, 5.0f);
	m_apply = new GUIButton("Apply", 350.0f, 600.0f, 200.0f, 61.0f, 5.0f);
	
	m_screen->add(m_cancel);
	m_screen->add(m_apply);
	
	guiSystem.add(m_screen);
}

void OptionsScreen::cleanup()
{
}

void OptionsScreen::update(float delta)
{
	if (m_cancel->wasClicked() || m_apply->wasClicked()) {
		m_menu->setState(MENUSTATE_MAIN);
	}
}

void OptionsScreen::setActive(bool active)
{
	m_screen->setActive(active);
}
