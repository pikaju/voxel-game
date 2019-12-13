#version 330

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D materialTexture;

const int NUM_SHADOW_MAPS = 4;

uniform sampler2DShadow shadowMap[NUM_SHADOW_MAPS];
uniform mat4 shadowViewProjectionMatrix[NUM_SHADOW_MAPS];

uniform vec3 lightDirection;
uniform vec3 cameraPosition;

in vec2 fragmentTexcoord;

layout(location = 0) out vec3 pixelColor;

float sampleShadow(vec2 textureCoordinate, float depth, int shadowIndex, float bias) {
	return texture(shadowMap[shadowIndex], vec3(textureCoordinate, depth - bias));
}

float getShadow(vec3 position, float bias)
{
	int shadowIndex = -1;
	vec4 shadowMapCoords;
	do {
		shadowIndex++;
		shadowMapCoords = shadowViewProjectionMatrix[shadowIndex] * vec4(position, 1.0);
	}
	while (shadowMapCoords.x < -1.0 || shadowMapCoords.x > 1.0 || shadowMapCoords.y < -1.0 || shadowMapCoords.y > 1.0);
	shadowMapCoords.xyz *= 0.5;
	shadowMapCoords.xyz += 0.5;
	return sampleShadow(shadowMapCoords.xy, shadowMapCoords.z, shadowIndex, bias * (shadowIndex + 1) * (shadowIndex + 1));
}

float getShadowPCF(vec3 position, float bias)
{
	int shadowIndex = -1;
	vec4 shadowMapCoords;
	do {
		shadowIndex++;
		shadowMapCoords = shadowViewProjectionMatrix[shadowIndex] * vec4(position, 1.0);
	}
	while (shadowMapCoords.x < -1.0 || shadowMapCoords.x > 1.0 || shadowMapCoords.y < -1.0 || shadowMapCoords.y > 1.0);

	shadowMapCoords.xyz *= 0.5;
	shadowMapCoords.xyz += 0.5;

	vec2 shadowMapSize = textureSize(shadowMap[shadowIndex], 0);

	float samples = 0.0;
	float sampleCount = 0.0;
	
	int sampleArea = NUM_SHADOW_MAPS - shadowIndex;
	sampleArea /= 2;
	sampleArea = 0;

	for (float x = -sampleArea; x <= sampleArea; x++) {
		for (float y = -sampleArea; y <= sampleArea; y++) {
			samples += sampleShadow(shadowMapCoords.xy + vec2(x, y) / shadowMapSize, shadowMapCoords.z, shadowIndex, bias * (shadowIndex + 1) * (shadowIndex + 1) * (sampleArea + 1) / shadowMapSize.x);
			sampleCount++;
		}
	}

	return samples / sampleCount;
}

float getDiffuseFactor(vec3 normal)
{
	float cosTheta = dot(normal, -lightDirection);
	return max(cosTheta, 0.0);
}

float getSpecularFactor(vec3 position, vec3 normal, float specularIntensity, float specularPower)
{
	vec3 cameraDirection = normalize(cameraPosition - position);
	vec3 reflectedLightDirection = normalize(reflect(lightDirection, normal));
	float intensity = dot(reflectedLightDirection, cameraDirection);
	return max(pow(intensity, specularPower), 0.0) * specularIntensity;
}

void main()
{
	vec3 fragmentPosition = texture(positionTexture, fragmentTexcoord).rgb;
	vec3 fragmentNormal = texture(normalTexture, fragmentTexcoord).rgb * 2.0 - 1.0;
	vec3 fragmentMaterial = texture(materialTexture, fragmentTexcoord).rgb;
	
	float cosTheta = dot(fragmentNormal, -lightDirection);

	float lightFactor = getDiffuseFactor(fragmentNormal) + getSpecularFactor(fragmentPosition, fragmentNormal, fragmentMaterial.r * 4.0, 32.0);
	lightFactor *= getShadowPCF(fragmentPosition, clamp(tan(acos(cosTheta)), 0.0, 32.0) * 1.0 / 16.0);

	vec3 light = vec3(0.8) * lightFactor;
	vec3 ambientLight = vec3(0.2 + dot(fragmentNormal, -normalize(vec3(0.3, -1.0, 0.5))) / 16.0);

	pixelColor = light + ambientLight;
}