#pragma once

#include <vt/graphics/shader/GLShaderProgram.h>
#include <vt/util/Camera.h>

#include "Shadow.h"

#define NUM_SHADOW_MAPS 4

class ChunkManager;

class CascadedShadow
{
public:
	CascadedShadow();
	~CascadedShadow();

	void initialize();
	void cleanup();

	void render(ChunkManager& chunkManager, const vt::GLShaderProgram& shader, const vt::Vec3f& cameraPosition, const vt::Vec3f& direction, float renderDistance);

	void setFrustum(const vt::Vec3f& cameraPosition, const vt::Vec3f& direction, float renderDistance);

	inline const Shadow& getShadow(unsigned int index) const { return m_shadows[index]; }
private:
	Shadow m_shadows[NUM_SHADOW_MAPS];
	int m_updateCycle;
};