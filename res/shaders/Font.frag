#version 330

uniform sampler2D fontTexture;

in vec2 fragmentTexcoord;

layout(location = 0) out vec4 pixelColor;

void main()
{
	pixelColor = texture(fontTexture, fragmentTexcoord);
}