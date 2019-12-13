#pragma once

#include "MainScreen.h"
#include "OptionsScreen.h"

class GUISystem;
class Game;

enum MenuState
{
	MENUSTATE_MAIN,
	MENUSTATE_OPTIONS
};

class Menu
{
public:
	Menu(Game* game);
	~Menu();

	void initialize(GUISystem& guiSystem);
	void cleanup();

	void update(float delta);

	inline void setState(const MenuState& state) { m_state = state; }

	inline Game* getGame() { return m_game; }
private:
	Game* m_game;
	MenuState m_state;
	MainScreen m_mainScreen;
	OptionsScreen m_optionsScreen;
};