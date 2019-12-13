#version 330

uniform sampler2D waterTexture;

uniform mat4 viewProjectionMatrix;
uniform float time;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec4 fragmentColor;
flat out vec3 fragmentNormal;

float sampleTexture(float x, float y)
{
	return texture(waterTexture, vec2(x, y) / 64.0).r * 2.0 - 1.0;
}

float getHeight(float x, float y)
{
	return (sampleTexture(x, y + time) + sampleTexture(x + time, y + time * 2.0) + sampleTexture(x / 2.0 - time, y / 4.0 - time / 2.0)) / 4.0;
}

void main()
{
	vec3 position = vertexPosition;
	float height = getHeight(position.x, position.z);
	position.y += height;
	
	fragmentPosition = position;
	fragmentColor = vertexColor;

	vec3 e0 = vec3(-1.0, getHeight(position.x - 1.0, position.z) - height, 0.0);
	vec3 e1 = vec3(0.0, getHeight(position.x, position.z - 1.0) - height, -1.0);
	fragmentNormal = normalize(cross(e1, e0));

	gl_Position = viewProjectionMatrix * vec4(position, 1.0);
}