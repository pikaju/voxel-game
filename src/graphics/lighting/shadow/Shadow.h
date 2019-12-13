#pragma once

#include <vt/graphics/texture/GLFramebuffer.h>
#include <vt/math/Mat4.h>

class Shadow
{
public:
	Shadow();
	~Shadow();

	void initialize(unsigned int width, unsigned int height);
	void cleanup();

	inline const vt::GLFramebuffer& getFramebuffer() const { return m_framebuffer; }
	inline const vt::Mat4f& getViewMatrix() const { return m_viewMatrix; }
	inline const vt::Mat4f& getProjectionMatrix() const { return m_projectionMatrix; }
	inline vt::Mat4f getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

	inline void setViewMatrix(const vt::Mat4f& viewMatrix) { m_viewMatrix = viewMatrix; }
	inline void setProjectionMatrix(const vt::Mat4f& projectionMatrix) { m_projectionMatrix = projectionMatrix; }

	inline bool hasChanged() const { return m_changed; }
	inline void change() { m_changed = true; }
	inline void update() { m_changed = false; }
private:
	vt::GLFramebuffer m_framebuffer;
	vt::Mat4f m_viewMatrix;
	vt::Mat4f m_projectionMatrix;
	bool m_changed;
};