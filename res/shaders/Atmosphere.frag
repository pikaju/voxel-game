#version 330

uniform vec3 sunDirection;

in vec3 fragmentDirection;

layout(location = 0) out vec4 pixelColor;

vec3 sampleAtmosphere(vec3 direction) {
	vec3 fogColor = mix(mix(vec3(1.0, 0.8, 0.3), vec3(0.05, 0.1, 0.2), -sunDirection.y), vec3(0.6, 0.8, 1.0), max(sunDirection.y, 0.0));
	return mix(fogColor, mix(vec3(0.3, 0.4, 0.8), vec3(0.07, 0.13, 0.23), -sunDirection.y), direction.y * 0.5 + 0.5);
}

vec3 sampleSun(vec3 direction)
{
	float a = dot(direction, sunDirection);
	return vec3(clamp(pow(a, 32) * a, 0.0, 1.0) * vec3(1.0, 0.8, 0.5)) * clamp((sunDirection.y + 0.2) * 32.0, 0.0, 1.0);
}

void main()
{
	vec3 direction = normalize(fragmentDirection);
	pixelColor = vec4(sampleAtmosphere(direction) + sampleSun(direction), 1.0);
}