#version 450

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout(location = 0) out vec2 fragOffset;

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

layout (push_constant) uniform Push
{
vec4 position;
vec4 color;
float radius;
} push;

void main()
{
	fragOffset = OFFSETS[gl_VertexIndex];

	vec4 lightInCameraSpace = ubo.view_matrix * vec4(push.position.xyz, 1.0);
	vec4 positionInCameraSpace = lightInCameraSpace + push.radius * vec4(fragOffset, 0.0, 0.0);

	gl_Position = ubo.projection_matrix * positionInCameraSpace;
}