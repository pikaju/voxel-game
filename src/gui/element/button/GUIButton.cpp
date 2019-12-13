#include "GUIButton.h"

GUIButton::GUIButton(const vt::String& text, float x, float y, float width, float height, float fontSize) : m_text(text), m_x(x), m_y(y), m_width(width), m_height(height), m_fontSize(fontSize)
{
}

GUIButton::~GUIButton()
{
}

void GUIButton::update(float delta, const vt::Input& input)
{
	m_hovering = input.getMouseX() >= m_x && input.getMouseY() >= m_y && input.getMouseX() < m_x + m_width && input.getMouseY() < m_y + m_height;
	m_clicked = m_pushed && !input.getButtonState(input.MOUSE_BUTTON_LEFT);
	m_pushed = m_hovering && input.getButtonState(input.MOUSE_BUTTON_LEFT);
}

void GUIButton::render(const vt::Window* window)
{
	float extraSize = 0.0f;
	if (m_hovering) extraSize += 1.0f;
	if (m_pushed) extraSize -= 0.5f;
	float xo = m_width / 2.0f - m_system->getFontRenderer().getStringWidth(m_text, m_fontSize + extraSize) / 2.0f;
	float yo = m_height / 2.0f - ((m_fontSize + extraSize) / 2.0f) * 9.0f;
	xo = yo;
	m_system->getFontRenderer().render(m_text, m_x + xo, m_y + yo, m_fontSize + extraSize);
}
