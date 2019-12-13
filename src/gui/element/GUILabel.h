#pragma once

#include "../GUIComponent.h"

class GUILabel : public GUIComponent
{
public:
	GUILabel(const vt::String& text, float x, float y, float fontSize);
	~GUILabel();

	virtual void render(const vt::Window* window) override;
private:
	vt::String m_text;
	float m_x;
	float m_y;
	float m_fontSize;
};