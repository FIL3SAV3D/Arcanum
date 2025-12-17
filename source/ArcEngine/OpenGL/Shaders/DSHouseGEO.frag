#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gORM;
layout (location = 4) out float gDepth;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D texture_diffuse1;

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


    vec3 normalMap = vec3(TBN[2]);
    //normalMap = normalMap * 2.0 - 1.0;
    gNormal = normalize(normalMap);

    gAlbedo = texture(texture_diffuse1, TexCoords);

    gORM = vec3(0.0, 0.7, 0.0);

    gDepth = LinearizeDepth(gl_FragCoord.z) / far;
}