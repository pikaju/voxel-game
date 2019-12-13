#pragma once

#include <vt/graphics/model/GLModel.h>
#include <vt/graphics/shader/GLShaderProgram.h>

class Stars
{
public:
	Stars();
	~Stars();

	void initialize();
	void cleanup();

	void render(const vt::Mat4f& viewProjectionMatrix, const vt::Vec3f& cameraPosition);
private:
	vt::GLShaderProgram m_shader;
	vt::GLModel m_model;
};