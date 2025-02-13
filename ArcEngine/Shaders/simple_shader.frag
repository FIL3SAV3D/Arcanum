#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;
layout (location = 3) in vec2 fragUV;


layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection_matrix;
	mat4 view_matrix;
	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
} ubo;

layout( binding = 1) uniform sampler2D texSampler;
layout( binding = 2) uniform sampler2D NormalTexSampler;

layout (push_constant) uniform Push
{
mat4 modelMatrix;
mat4 normalMatrix;
} push;

void main()
{
	vec3 directionToLight = ubo.lightPosition - fragPosWorld;
	float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;

	vec3 sampledNormals = texture(NormalTexSampler, fragUV).rgb * fragNormalWorld;

	vec3 diffuseLight = lightColor * max(dot(normalize(sampledNormals) * 3.0, normalize(directionToLight)), 0);

    outColor = vec4((diffuseLight + ambientLight) * texture(texSampler, fragUV).rgb , 1.0);
//	outColor = vec4(sampledNormals, 1.0);
}