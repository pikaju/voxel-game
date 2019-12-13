#pragma once

#include "../../GUIComponent.h"

class GUIButton : public GUIComponent
{
public:
	GUIButton(const vt::String& text, float x, float y, float width, float height, float fontSize);
	~GUIButton();
	
	virtual void update(float delta, const vt::Input& input) override;
	virtual void render(const vt::Window* window) override;

	inline bool wasClicked() { return m_clicked; }
private:
	vt::String m_text;
	float m_x;
	float m_y;
	float m_width;
	float m_height;
	float m_fontSize;
	bool m_hovering;
	bool m_pushed;
	bool m_clicked;
};