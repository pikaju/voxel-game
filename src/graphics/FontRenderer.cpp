#include "FontRenderer.h"

#include <vt/graphics/model/Attribute.h>
#include <vt/graphics/texture/SpriteSheet.h>
#include <vt/io/ImageLoader.h>

FontRenderer::FontRenderer() : m_renderer(), m_spriteSheet(), m_shader(), m_characters(), m_characterSizes()
{
}

FontRenderer::~FontRenderer()
{
}

void FontRenderer::initialize()
{
	m_renderer.initialize();
	std::vector<vt::Attribute> attributes;
	attributes.push_back(vt::Attribute(vt::TYPE_FLOAT, 2));
	attributes.push_back(vt::Attribute(vt::TYPE_FLOAT, 2));
	m_renderer.allocate(128 * 3, attributes.data(), attributes.size());

	m_spriteSheet.initialize((vt::Texture*)vt::io::loadGLTexture("res/textures/Font.png", vt::FILTER_NEAREST), 8, 16);

	m_shader.initialize();
	m_shader.loadShader("res/shaders/Font.vert", vt::SHADERTYPE_VERTEX);
	m_shader.loadShader("res/shaders/Font.frag", vt::SHADERTYPE_FRAGMENT);
	m_shader.link();

	m_characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,?!:;/\\-_+*|=()$%'#0123456789<>";
	m_characterSizes = "555555555555555555555555555555545514415555545355555511511133455314225515535555555533";
}

void FontRenderer::cleanup()
{
	m_shader.cleanup();
	m_spriteSheet.cleanup();
	m_renderer.cleanup();
}

void FontRenderer::begin()
{
	m_renderer.setDepthRead(false);
	m_renderer.setBlending(vt::BLENDING_DEFAULT);
	m_spriteSheet.getTexture()->enable(0);
	m_shader.enable();
	m_shader.setUniform("fontTexture", 0);
	m_renderer.begin();
}

void FontRenderer::end()
{
	m_renderer.end();
	m_shader.disable();
	m_renderer.setBlending(vt::BLENDING_DISABLED);
	m_renderer.setDepthRead(true);
}

void FontRenderer::render(const vt::String& text, float x, float y, float scale)
{
	begin();
	char c;
	unsigned int index = 0;
	float xo = 0;
	float yo = 0;

	while ((c = text[index]) != '\0') {
		index++;
		
		if (c == ' ') {
			xo += 3.0f * scale;
			continue;
		}

		unsigned int sprite = m_characters.find(c);
		unsigned int cw = m_characterSizes[sprite] - '0';

		float xx = (x + xo) / (float)m_screenWidth * 2.0f - 1.0f;
		float yy = (y + yo) / (float)m_screenHeight * -2.0f + 1.0f;
		float xs = 8.0f * scale / (float)m_screenWidth * 2.0f;
		float ys = 16.0f * scale / (float)m_screenHeight * -2.0f;
		xo += (float)(cw + 1) * scale;

		FontVertex v0(vt::Vec2f(xx, yy + ys), m_spriteSheet.getTextureCoordinates(sprite, vt::SPRITECORNER_BL));
		FontVertex v1(vt::Vec2f(xx + xs, yy + ys), m_spriteSheet.getTextureCoordinates(sprite, vt::SPRITECORNER_BR));
		FontVertex v2(vt::Vec2f(xx + xs, yy), m_spriteSheet.getTextureCoordinates(sprite, vt::SPRITECORNER_TR));
		FontVertex v3(vt::Vec2f(xx, yy), m_spriteSheet.getTextureCoordinates(sprite, vt::SPRITECORNER_TL));

		m_renderer.submit(&v0);
		m_renderer.submit(&v1);
		m_renderer.submit(&v2);

		m_renderer.submit(&v0);
		m_renderer.submit(&v2);
		m_renderer.submit(&v3);
	}
	end();
}

float FontRenderer::getStringWidth(const vt::String& text, float scale)
{
	char c;
	unsigned int index = 0;
	float xo = 0;

	while ((c = text[index]) != '\0') {
		index++;

		if (c == ' ') {
			xo += 3.0f * scale;
			continue;
		}
		unsigned int sprite = m_characters.find(c);
		unsigned int cw = m_characterSizes[sprite] - '0';

		xo += (float)(cw + 1) * scale;
	}
	return xo - scale;
}
