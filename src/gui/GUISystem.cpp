#include "GUISystem.h"

#include <vt/graphics/window/Window.h>

#include "GUIComponent.h"

GUISystem::GUISystem() : m_components(), m_fontRenderer()
{
}

GUISystem::~GUISystem()
{
}

void GUISystem::initialize()
{
	m_fontRenderer.initialize();
}

void GUISystem::cleanup()
{
	for (unsigned int i = 0; i < m_components.size(); i++) {
		m_components[i]->cleanup();
		delete m_components[i];
	}
	m_fontRenderer.cleanup();
}

void GUISystem::update(float delta, const vt::Input& input)
{
	for (unsigned int i = 0; i < m_components.size(); i++) {
		if (m_components[i]->isActive()) {
			m_components[i]->update(delta, input);
		}
	}
}

void GUISystem::render(const vt::Window* window)
{
	m_fontRenderer.setScreenSize(window->getWidth(), window->getHeight());
	for (unsigned int i = 0; i < m_components.size(); i++) {
		if (m_components[i]->isActive()) {
			m_components[i]->render(window);
		}
	}
}

void GUISystem::add(GUIComponent* component)
{
	component->setSystem(this);
	component->initialize();
	m_components.push_back(component);
}
