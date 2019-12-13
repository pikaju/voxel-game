#pragma once

#include "shadow/CascadedShadow.h"

class LightManager
{
public:
	LightManager();
	~LightManager();

	void initialize();
	void cleanup();

	void render(ChunkManager& chunkManager, const vt::GLShaderProgram& shader, const vt::Vec3f& cameraPosition, float renderDistance, const vt::GLFramebuffer& sceneBuffer, unsigned int positionTexture, unsigned int normalTexture, unsigned int materialTexture);

	void setScreenSize(unsigned int width, unsigned int height);

	inline const vt::GLFramebuffer& getFramebuffer() const { return m_framebuffer; }
	inline const vt::Vec3f& getDirectionalLightDirection() { return m_directionalLightDirection; }
	inline void setDirectionalLightDirection(const vt::Vec3f& directionalLightDirection) { m_directionalLightDirection = directionalLightDirection; }
private:
	vt::GLFramebuffer m_framebuffer;
	CascadedShadow m_cascadedShadow;
	vt::GLShaderProgram m_directionalLightShader;
	vt::Vec3f m_directionalLightDirection;
};