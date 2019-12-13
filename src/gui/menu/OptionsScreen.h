#pragma once

class Menu;
class GUISystem;
class GUIScreen;
class GUIButton;

class OptionsScreen
{
public:
	OptionsScreen(Menu* menu);
	~OptionsScreen();

	void initialize(GUISystem& guiSystem);
	void cleanup();

	void update(float delta);

	void setActive(bool active);
private:
	Menu* m_menu;
	GUIScreen* m_screen;
	GUIButton* m_cancel;
	GUIButton* m_apply;
};