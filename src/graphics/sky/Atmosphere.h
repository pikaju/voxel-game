#pragma once

#include <vt/graphics/shader/GLShaderProgram.h>

class Atmosphere
{
public:
	Atmosphere();
	~Atmosphere();

	void initialize();
	void cleanup();

	void render(const vt::Mat4f& inverseViewProjectionMatrix, const vt::Vec3f& cameraPosition, const vt::Vec3f& sunDirection);
private:
	vt::GLShaderProgram m_shader;
};