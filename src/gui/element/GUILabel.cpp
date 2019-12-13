#include "GUILabel.h"

GUILabel::GUILabel(const vt::String& text, float x, float y, float fontSize) : m_text(text), m_x(x), m_y(y), m_fontSize(fontSize)
{
}

GUILabel::~GUILabel()
{
}

void GUILabel::render(const vt::Window* window)
{
	m_system->getFontRenderer().render(m_text, m_x, m_y, m_fontSize);
}
