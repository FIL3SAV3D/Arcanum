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
	vec4 camPos;
	vec4 ambientLightColor;
	pointLight pointLights[10];
	int numLights;
} ubo;

const float minPixelBetweenCells = 2.0;
const float gGridCellSize = 0.1;
const vec4  gGridColorThin = vec4(0.05, 0.1, 0.05, 1.0);
const vec4  gGridColorThick = vec4(0.05, 0.3, 0.05, 1.0);

vec2 satv(vec2 _input)
{
	float one = clamp(_input.x, 0.0, 1.0);
	float two = clamp(_input.y, 0.0, 1.0);
	return vec2(one, two);
}

float saturate(float _INPUT)
{
	float OUTPUT = clamp(_INPUT, 0.0, 1.0);
	return OUTPUT;
}

float log10(float _x)
{
	float f = log(_x) / log(10.0);
	return f;
}

void main()
{
	vec2 dvx = vec2(dFdx(worldPos.x), dFdy(worldPos.x));
	vec2 dvy = vec2(dFdx(worldPos.z), dFdy(worldPos.z));

	float lx = length(dvx);
	float ly = length(dvy);

	vec2 dudv = vec2(lx, ly);

	float l = length(dudv);

	float LOD = max(0.0, log10(l * minPixelBetweenCells / gGridCellSize) + 1.0);

	float gridCellSizeLod0 = gGridCellSize * pow(10.0, floor(LOD));
	float gridCellSizeLod1 = gridCellSizeLod0 * 10.0;
	float gridCellSizeLod2 = gridCellSizeLod1 * 10.0;

	dudv *= 4.0;

	//float Lod0a = mod(worldPos.z, gGridCellSize) / (4.0 * dFdy(worldPos.z));

	vec2 mod_div_dudv = mod(worldPos.xz, gridCellSizeLod0) / dudv;
	float Lod0a = max(1.0 - abs(saturate(mod_div_dudv.x) * 2.0 - 1.0), 1.0 - abs(saturate(mod_div_dudv.y) * 2.0 - 1.0));

	mod_div_dudv = mod(worldPos.xz, gridCellSizeLod1) / dudv;
	float Lod1a = max(1.0 - abs(saturate(mod_div_dudv.x) * 2.0 - 1.0), 1.0 - abs(saturate(mod_div_dudv.y) * 2.0 - 1.0));

	mod_div_dudv = mod(worldPos.xz, gridCellSizeLod2) / dudv;
	float Lod2a = max(1.0 - abs(saturate(mod_div_dudv.x) * 2.0 - 1.0), 1.0 - abs(saturate(mod_div_dudv.y) * 2.0 - 1.0));

	float LOD_fade = fract(LOD);

	vec4 Color;

	if(Lod2a > 0.0)
	{
		Color = gGridColorThick;
		Color.a *= Lod2a;
	}
	else
	{
		if(Lod1a > 0.0)
		{
			Color = mix(gGridColorThick, gGridColorThin, LOD_fade);
			Color.a *= Lod1a;
		}
		else
		{
			Color = gGridColorThin;
			Color.a *= Lod0a * (1.0 - LOD_fade);
		}
	}

	float opacity_falloff = (1.0 - saturate(length(worldPos.xz - ubo.camPos.xz) / 100.0));

	Color.a *= opacity_falloff;

	outColor = Color;
}