#include "CascadedShadow.h"

#include <vt/graphics/renderer/GLRenderer.h>

#include "../../../world/chunk/ChunkManager.h"

CascadedShadow::CascadedShadow() : m_updateCycle(0)
{
}

CascadedShadow::~CascadedShadow()
{
}

void CascadedShadow::initialize()
{
	for (unsigned int i = 0; i < NUM_SHADOW_MAPS; i++) {
		m_shadows[i].initialize(2048, 2048);
	}
}

void CascadedShadow::cleanup()
{
	for (unsigned int i = 0; i < NUM_SHADOW_MAPS; i++) {
		m_shadows[i].cleanup();
	}
}

void CascadedShadow::render(ChunkManager& chunkManager, const vt::GLShaderProgram& shader, const vt::Vec3f& cameraPosition, const vt::Vec3f& direction, float renderDistance)
{
	setFrustum(cameraPosition, direction, renderDistance);
	vt::GLRenderer renderer;
	renderer.setCulling(vt::CULLING_DISABLED);
	for (unsigned int i = 0; i < NUM_SHADOW_MAPS; i++) {
		if (!m_shadows[i].hasChanged()) {
			continue;
		}
		m_shadows[i].getFramebuffer().bind();
		renderer.clear(false, true, false);
		chunkManager.render(shader, m_shadows[i].getViewMatrix(), m_shadows[i].getProjectionMatrix());
		m_shadows[i].update();
	}
	renderer.setCulling(vt::CULLING_BACK);
}

void CascadedShadow::setFrustum(const vt::Vec3f& cameraPosition, const vt::Vec3f& direction, float renderDistance)
{
	for (unsigned int i = 0; i < NUM_SHADOW_MAPS; i++) {
		if (m_updateCycle % ((i + 1) * (i + 1) * (i + 1) + 1) != 0) {
			continue;
		}
		
		float size = (i + 1) * (i + 1) * (renderDistance / (NUM_SHADOW_MAPS * NUM_SHADOW_MAPS));

		vt::Vec3f right = direction.cross(vt::Vec3f(0.0f, 1.0f, 0.0f)).normalized();
		vt::Vec3f up = right.cross(direction).normalized();
		m_shadows[i].setViewMatrix(vt::Mat4f::lookat(right, up, direction * -1.0f));

		vt::Vec4f projectedCameraPosition = m_shadows[i].getViewMatrix() * vt::Vec4f(cameraPosition, 1.0f);
		projectedCameraPosition /= projectedCameraPosition.w;

		float texelWidth = size * 2.0f / (float)m_shadows[i].getFramebuffer().getWidth();
		float texelHeight = size * 2.0f / (float)m_shadows[i].getFramebuffer().getHeight();

		projectedCameraPosition.x = floor(projectedCameraPosition.x / texelWidth) * texelWidth;
		projectedCameraPosition.y = floor(projectedCameraPosition.y / texelHeight) * texelHeight;

		m_shadows[i].setProjectionMatrix(vt::Mat4f::ortho(projectedCameraPosition.x - size, projectedCameraPosition.x + size, projectedCameraPosition.y - size, projectedCameraPosition.y + size, 512.0f + projectedCameraPosition.z, -512.0f + projectedCameraPosition.z));
		m_shadows[i].change();
	}

	m_updateCycle++;
	if (m_updateCycle > NUM_SHADOW_MAPS * NUM_SHADOW_MAPS * NUM_SHADOW_MAPS + 1) {
		m_updateCycle = 0;
	}
}
