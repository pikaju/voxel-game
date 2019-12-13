#version 330

uniform mat4 viewProjectionMatrix;
uniform float time;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec3 fragmentNormal;

void main()
{
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	fragmentNormal = vertexNormal;
	gl_Position = viewProjectionMatrix * vec4(vertexPosition, 1.0);
}