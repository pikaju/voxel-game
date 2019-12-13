#version 330

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

layout(location = 0) in vec3 vertexPosition;

void main()
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}