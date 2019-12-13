#version 330

uniform mat4 inverseViewProjectionMatrix;
uniform mat4 previousViewProjectionMatrix;
uniform sampler2D colorTexture;
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D lightTexture;

uniform vec3 cameraPosition;
uniform float fogDistance;
uniform float delta;

in vec2 fragmentTexcoord;

layout(location = 0) out vec4 pixelColor;

vec2 calculateMotionBlurVector(vec3 fragmentPosition, vec2 fragmentTexcoord)
{
	vec4 previousFragmentTexcoord = previousViewProjectionMatrix * vec4(fragmentPosition, 1.0);
	previousFragmentTexcoord.xy = previousFragmentTexcoord.xy / previousFragmentTexcoord.w * 0.5 + 0.5;
	return fragmentTexcoord - previousFragmentTexcoord.xy;
}

vec3 calculateMotionBlurColor(vec3 fragmentPosition, vec2 fragmentTexcoord)
{
	vec2 blurVector = calculateMotionBlurVector(fragmentPosition, fragmentTexcoord);
	const float MOTION_BLUR_SAMPLES = 4.0;
	vec3 avarageColor = vec3(0.0);

	for (float i = -MOTION_BLUR_SAMPLES / 2.0 + 0.5; i < MOTION_BLUR_SAMPLES / 2.0; i += 1.0) {
		vec2 texcoord = fragmentTexcoord + blurVector * (-i / MOTION_BLUR_SAMPLES) / delta / 64.0;
		avarageColor += texture(colorTexture, texcoord).rgb * texture(lightTexture, texcoord).rgb;
	}

	return avarageColor / MOTION_BLUR_SAMPLES;
}

float calculateOutline(vec3 fragmentPosition, vec2 fragmentTexcoord)
{
	vec2 size = textureSize(positionTexture, 0);
	float d0 = length(texture(positionTexture, fragmentTexcoord + vec2(1.0, 0.0) / size).rgb - fragmentPosition);
	float d1 = length(texture(positionTexture, fragmentTexcoord + vec2(0.0, 1.0) / size).rgb - fragmentPosition);
	return max(1.0 - max(d0, d1) / 8.0, 0.0);
}

float calculateFog(vec3 fragmentPosition)
{
	vec3 direction = fragmentPosition - cameraPosition;
	float l = length(direction);
	return l / 256.0;
}

void main()
{
	float fragmentDepth = texture(depthTexture, fragmentTexcoord).r;
	vec3 fragmentPosition = texture(positionTexture, fragmentTexcoord).rgb;
	vec3 fragmentColor = calculateMotionBlurColor(fragmentPosition, fragmentTexcoord);
	vec3 fragmentNormal = texture(normalTexture, fragmentTexcoord).rgb * 2.0 - 1.0;
	vec3 fragmentDirection = normalize(fragmentPosition - cameraPosition);

	vec4 fragmentPositionUnprojected = inverseViewProjectionMatrix * vec4(fragmentTexcoord * 2.0 - 1.0, fragmentDepth * 2.0 - 1.0, 1.0);
	float distanceToCamera = length(cameraPosition - fragmentPositionUnprojected.xyz / fragmentPositionUnprojected.w);
	
	float fog = clamp(distanceToCamera / fogDistance, 0.0, 1.0);
	fog = pow(fog, 4);
	if (distanceToCamera > fogDistance) {
		discard;
	}

	vec3 color = fragmentColor * calculateOutline(fragmentPosition, fragmentTexcoord);

	pixelColor = vec4(color, 1.0);
}