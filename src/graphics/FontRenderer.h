#pragma once

#include <vt/graphics/renderer/GLBatchRenderer.h>
#include <vt/graphics/texture/SpriteSheet.h>
#include <vt/graphics/shader/GLShaderProgram.h>
#include <vt/math/Vec2.h>
#include <vt/util/String.h>

class FontVertex
{
public:
	FontVertex(const vt::Vec2f& position = vt::Vec2f(), const vt::Vec2f& texcoord = vt::Vec2f()) : m_position(position), m_texcoord(texcoord) {  }
	~FontVertex() {  }
private:
	vt::Vec2f m_position;
	vt::Vec2f m_texcoord;
};

class FontRenderer
{
public:
	FontRenderer();
	~FontRenderer();

	void initialize();
	void cleanup();

	void begin();
	void end();

	void render(const vt::String& text, float x, float y, float scale);

	float getStringWidth(const vt::String& text, float scale);

	inline void setScreenSize(unsigned int width, unsigned int height) { m_screenWidth = width; m_screenHeight = height; }
private:
	vt::GLBatchRenderer m_renderer;
	vt::SpriteSheet m_spriteSheet;
	vt::GLShaderProgram m_shader;
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
	std::string m_characters;
	std::string m_characterSizes;
};