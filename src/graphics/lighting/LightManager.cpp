#include "LightManager.h"

#include <vt/graphics/renderer/GLAttributelessRenderer.h>

#include "../../world/chunk/ChunkManager.h"

LightManager::LightManager() : m_framebuffer(), m_cascadedShadow(), m_directionalLightShader(), m_directionalLightDirection()
{
}

LightManager::~LightManager()
{
}

void LightManager::initialize()
{
	m_cascadedShadow.initialize();
	m_directionalLightShader.initialize();
	m_directionalLightShader.loadShader("res/shaders/DirectionalLight.vert", vt::SHADERTYPE_VERTEX);
	m_directionalLightShader.loadShader("res/shaders/DirectionalLight.frag", vt::SHADERTYPE_FRAGMENT);
	m_directionalLightShader.link();
}

void LightManager::cleanup()
{
	m_directionalLightShader.cleanup();
	m_cascadedShadow.cleanup();
	m_framebuffer.cleanup();
}

void LightManager::render(ChunkManager& chunkManager, const vt::GLShaderProgram& shader, const vt::Vec3f& cameraPosition, float renderDistance, const vt::GLFramebuffer& sceneBuffer, unsigned int positionTexture, unsigned int normalTexture, unsigned int materialTexture)
{
	vt::GLAttributelessRenderer renderer;

	m_cascadedShadow.render(chunkManager, shader, cameraPosition, m_directionalLightDirection, renderDistance);

	m_framebuffer.bind();

	sceneBuffer.use(positionTexture, 0);
	sceneBuffer.use(normalTexture, 1);
	sceneBuffer.use(materialTexture, 2);
	for (unsigned int i = 0; i < NUM_SHADOW_MAPS; i++) {
		m_cascadedShadow.getShadow(i).getFramebuffer().use(0, 3 + i);
	}

	m_directionalLightShader.enable();
	m_directionalLightShader.setUniform("positionTexture", 0);
	m_directionalLightShader.setUniform("normalTexture", 1);
	m_directionalLightShader.setUniform("materialTexture", 2);

	for (int i = 0; i < NUM_SHADOW_MAPS; i++) {
		m_directionalLightShader.setUniform(("shadowMap[" + vt::toString(i) + "]").c_str(), 3 + i);
		m_directionalLightShader.setUniform(("shadowViewProjectionMatrix[" + vt::toString(i) + "]").c_str(), m_cascadedShadow.getShadow(i).getViewProjectionMatrix());
	}

	m_directionalLightShader.setUniform("lightDirection", m_directionalLightDirection);
	m_directionalLightShader.setUniform("cameraPosition", cameraPosition);

	renderer.render(vt::PRIMITIVE_TRIANGLE_STRIP, 4);

	m_directionalLightShader.disable();
}

void LightManager::setScreenSize(unsigned int width, unsigned int height)
{
	std::vector<vt::Attachment> attachments;
	attachments.push_back(vt::ATTACHMENT_COLOR);
	std::vector<vt::Format> formats;
	formats.push_back(vt::FORMAT_RGB32F);
	m_framebuffer.cleanup();
	m_framebuffer.initialize(width, height, attachments.size(), attachments.data(), formats.data());
}
