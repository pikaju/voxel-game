#version 330

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexTexcoord;

out vec2 fragmentTexcoord;

void main()
{
	fragmentTexcoord = vertexTexcoord;
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
}