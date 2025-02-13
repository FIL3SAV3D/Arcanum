#version 450

layout (location = 0) in vec2 fragOffset;

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

layout (push_constant) uniform Push
{
vec4 position;
vec4 color;
float radius;
} push;

void main()
{
	float dist = sqrt(dot(fragOffset, fragOffset));
	if(dist >= 1.0)
	{
		discard;
	}
	outColor = vec4(push.color.xyz, 1.0);
}