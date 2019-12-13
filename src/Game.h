#pragma once

#include <vt/graphics/window/Window.h>
#include <vt/graphics/window/Input.h>

#include "world/World.h"
#include "gui/GUISystem.h"
#include "gui/menu/Menu.h"

enum GameState
{
	GAMESTATE_MENU,
	GAMESTATE_WORLD
};

class Game
{
public:
	Game();
	~Game();

	void initialize();
	void cleanup();

	void update(float delta, const vt::Input& input);
	void render(float delta, const vt::Window* window);

	inline void setState(const GameState& state) { m_state = state; }
private:
	GameState m_state;
	World m_world;
	GUISystem m_guiSystem;
	Menu m_menu;
};