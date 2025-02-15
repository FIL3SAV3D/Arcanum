#version 450

const vec3 Pos[4] = vec3[4]
(
	vec3(-1.0, 0.0, -1.0),
	vec3( 1.0, 0.0, -1.0),
	vec3( 1.0, 0.0,  1.0),
	vec3(-1.0, 0.0,  1.0)
);

const int Indicies[6] = int[6](0, 2, 1, 2, 0, 3);

layout(location = 0) out vec3 worldPos;

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
	vec4 camPos;
	vec4 ambientLightColor;
	pointLight pointLights[10];
	int numLights;
} ubo;

void main()
{
	int Index = Indicies[gl_VertexIndex];
	vec3 vPos3 = Pos[Index] * 100.0;

	vPos3.x += ubo.camPos.x;
	vPos3.z += ubo.camPos.z;

	vec4 vPos4 = vec4(vPos3, 1.0);

	gl_Position = ubo.projection_matrix * ubo.view_matrix * vPos4;

	worldPos = vPos3;
}