#include "GUIScreen.h"

GUIScreen::GUIScreen() : m_components()
{
}

GUIScreen::~GUIScreen()
{
}

void GUIScreen::initialize()
{
	for (unsigned int i = 0; i < m_components.size(); i++) {
		m_system->add(m_components[i]);
	}
}

void GUIScreen::setActive(bool active)
{
	for (unsigned int i = 0; i < m_components.size(); i++) {
		m_components[i]->setActive(active);
	}
}
