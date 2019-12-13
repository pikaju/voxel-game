#version 330

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;

layout(location = 0) out vec4 pixelColor;
layout(location = 1) out vec3 pixelPosition;
layout(location = 2) out vec3 pixelNormal;
layout(location = 3) out vec3 pixelMaterial;
layout(location = 4) out float pixelDepth;

void main()
{
	pixelColor = fragmentColor;
	pixelPosition = fragmentPosition;
	pixelNormal = fragmentNormal * 0.5 + 0.5;
	pixelMaterial = vec3(0.0, 0.0, 0.0);
	pixelDepth = gl_FragCoord.z;
}