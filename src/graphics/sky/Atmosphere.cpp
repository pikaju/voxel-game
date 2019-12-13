#include "Atmosphere.h"

#include <vt/graphics/renderer/GLAttributelessRenderer.h>

Atmosphere::Atmosphere() : m_shader()
{
}

Atmosphere::~Atmosphere()
{
}

void Atmosphere::initialize()
{
	m_shader.initialize();
	m_shader.loadShader("res/shaders/Atmosphere.vert", vt::SHADERTYPE_VERTEX);
	m_shader.loadShader("res/shaders/Atmosphere.frag", vt::SHADERTYPE_FRAGMENT);
	m_shader.link();
}

void Atmosphere::cleanup()
{
	m_shader.cleanup();
}

void Atmosphere::render(const vt::Mat4f& inverseViewProjectionMatrix, const vt::Vec3f& cameraPosition, const vt::Vec3f& sunDirection)
{
	vt::GLAttributelessRenderer renderer;
	renderer.setDepthWrite(false);
	m_shader.enable();
	m_shader.setUniform("inverseViewProjectionMatrix", inverseViewProjectionMatrix);
	m_shader.setUniform("cameraPosition", cameraPosition);
	m_shader.setUniform("sunDirection", sunDirection);
	renderer.render(vt::PRIMITIVE_TRIANGLE_STRIP, 4);
	m_shader.disable();
	renderer.setDepthWrite(true);
}
