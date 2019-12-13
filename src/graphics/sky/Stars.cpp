#include "Stars.h"

#include <vt/graphics/model/Mesh.h>
#include <vt/graphics/renderer/GLRenderer.h>

Stars::Stars() : m_shader(), m_model()
{
}

Stars::~Stars()
{
}

void Stars::initialize()
{
	m_shader.initialize();
	m_shader.loadShader("res/shaders/Star.vert", vt::SHADERTYPE_VERTEX);
	m_shader.loadShader("res/shaders/Star.frag", vt::SHADERTYPE_FRAGMENT);
	m_shader.link();

	m_model.initialize();
	vt::Mesh<vt::Vec3f> mesh;
	mesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));

	const float radius = 1.0f / 256.0f;
	for (unsigned int i = 0; i < 1024; i++) {
		vt::Vec3f direction((float)(rand() % 32768) - (float)(32768 / 2), (float)(rand() % 32768) - (float)(32768 / 2), (float)(rand() % 32768) - (float)(32768 / 2));
		direction.normalize();
		vt::Vec3f tangent = direction.cross(vt::Vec3f(0.0f, 1.0f, 0.0f)).normalized();
		vt::Vec3f bitangent = tangent.cross(direction).normalized();
		vt::Vec3f v0 = direction + tangent * radius + bitangent * radius;
		vt::Vec3f v1 = direction + tangent * -radius + bitangent * radius;
		vt::Vec3f v2 = direction + tangent * -radius + bitangent * -radius;
		vt::Vec3f v3 = direction + tangent * radius + bitangent * -radius;
		mesh.quad(v0, v1, v2, v3);
	}
	
	mesh.createGLModel(m_model);
}

void Stars::cleanup()
{
	m_model.cleanup();
	m_shader.cleanup();
}

void Stars::render(const vt::Mat4f& viewProjectionMatrix, const vt::Vec3f& cameraPosition)
{
	vt::GLRenderer renderer;
	renderer.setDepthRead(true);
	m_shader.enable();
	m_shader.setUniform("modelMatrix", vt::Mat4f::translation(cameraPosition.x, cameraPosition.y, cameraPosition.z));
	m_shader.setUniform("viewProjectionMatrix", viewProjectionMatrix);
	m_model.render();
	m_shader.disable();
	renderer.setDepthRead(true);
}
