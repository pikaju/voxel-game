#pragma once

class Menu;
class GUISystem;
class GUIScreen;
class GUIButton;

class MainScreen
{
public:
	MainScreen(Menu* menu);
	~MainScreen();

	void initialize(GUISystem& guiSystem);
	void cleanup();

	void update(float delta);

	void setActive(bool active);
private:
	Menu* m_menu;
	GUIScreen* m_screen;
	GUIButton* m_play;
	GUIButton* m_options;
	GUIButton* m_quit;
};