#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;
layout (location = 3) in vec2 fragUV;


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
	vec4 ambientLightColor;
	pointLight pointLights[10];
	int numLights;
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
	vec3 sampledNormals = texture(NormalTexSampler, fragUV).rgb * fragNormalWorld;

	vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	//vec3 surfaceNormal = normalize(fragNormalWorld); Use this when no textures instead of sampledNormals

	for(int i = 0; i < ubo.numLights; i++)
	{
		pointLight light = ubo.pointLights[i];
		vec3 directionToLight = light.position.xyz - fragPosWorld;
		float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
		float cosAngIncidence = max(dot(normalize(sampledNormals) * 3.0, normalize(directionToLight)), 0);
		vec3 intensity = light.color.xyz * light.color.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;

		outColor = vec4(diffuseLight * texture(texSampler, fragUV).rgb , 1.0);
	}
}