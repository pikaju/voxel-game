#pragma once

#include <vector>

#include <vt/graphics/window/Input.h>

#include "../graphics/FontRenderer.h"

class GUIComponent;

class GUISystem
{
public:
	GUISystem();
	~GUISystem();

	void initialize();
	void cleanup();

	void update(float delta, const vt::Input& input);
	void render(const vt::Window* window);

	void add(GUIComponent* component);

	inline FontRenderer& getFontRenderer() { return m_fontRenderer; }
private:
	std::vector<GUIComponent*> m_components;
	FontRenderer m_fontRenderer;
};