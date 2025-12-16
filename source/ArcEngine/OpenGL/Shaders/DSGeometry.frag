#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gORM;
layout (location = 4) out float gDepth;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ORM1;

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    gPosition = FragPos;

    vec3 normalMap = texture(texture_normal1, TexCoords).rgb;
    normalMap = normalMap * 2.0 - 1.0;
    gNormal = normalize(Normal * normalMap);

    gAlbedo = texture(texture_diffuse1, TexCoords);

    gORM = texture(texture_ORM1, TexCoords).rgb;

    gDepth = LinearizeDepth(gl_FragCoord.z) / far;

    if(gAlbedo.a < 0.5)
        discard;
}