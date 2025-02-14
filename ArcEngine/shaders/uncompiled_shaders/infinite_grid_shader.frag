#version 450

layout (location = 0) in vec3 worldPos;

layout (location = 0) out vec4 outColor;

struct pointLight
{
	vec4 position; // ignore w
	vec4 color; // w is intensity
};

layout (set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection_matrix;
	mat4 view_matrix;
	mat4 inverse_view_matrix;
	vec4 ambientLightColor;
	pointLight pointLights[10];
	int numLights;
} ubo;

const float gGridCellSize = 0.5;
const vec4  gGridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
const vec4  gGridColorThick = vec4(0.1, 0.8, 0.1, 1.0);

void main()
{
	float Lod0a = mod(worldPos.z, gGridCellSize) + mod(worldPos.x, gGridCellSize);

	vec4 Color;

	Color = gGridColorThick;
	Color.a *= Lod0a;

	outColor = Color;
}