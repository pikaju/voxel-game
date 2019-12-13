#version 330

uniform mat4 inverseViewProjectionMatrix;
uniform vec3 cameraPosition;

out vec3 fragmentDirection;

const vec2 vertices[4] = vec2[] (vec2(-1.0, 1.0), vec2(-1.0, -1.0), vec2(1.0, 1.0), vec2(1.0, -1.0));

void main()
{
	vec4 direction = inverseViewProjectionMatrix * vec4(vertices[gl_VertexID], -1.0, 1.0);
	fragmentDirection = direction.xyz / direction.w - cameraPosition;
	gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}