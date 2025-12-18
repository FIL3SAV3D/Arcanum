#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gORM;
layout (location = 4) out float gDepth;

in vec3 WorldPos;
in vec2 TexCoords;
in mat3 TBN;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_ORM;

float near = 0.1;
float far  = 1000.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    gPosition = WorldPos;

    vec3 normalMap = texture(texture_normal, TexCoords).rgb;
    normalMap = normalMap * 2.0 - 1.0;
    gNormal = normalize(TBN * normalMap);

    gAlbedo = texture(texture_albedo, TexCoords);

    gORM = texture(texture_ORM, TexCoords).rgb;

    gDepth = LinearizeDepth(gl_FragCoord.z) / far;

    if(gAlbedo.a < 0.5)
        discard;
}