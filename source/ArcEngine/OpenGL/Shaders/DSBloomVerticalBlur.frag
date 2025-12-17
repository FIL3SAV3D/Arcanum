#version 460 core

in vec2 TexCoords;

uniform sampler2D gScene;
uniform sampler2D customInput01;
uniform sampler2D customInput02;

layout (location = 0) out vec4 customOutput01;
layout (location = 1) out vec4 customOutput02;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform float mul = 10.0;

void main()
{ 
    vec2 tex_offset = 1.0 / textureSize(customInput01, 0); // gets size of single texel
    vec3 result = texture(customInput01, TexCoords).rgb * weight[0]; // current fragment's contribution

    for(int i = 1; i < 5; ++i)
    {
        result += texture(customInput01, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        result += texture(customInput01, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    }

    customOutput01 = vec4(result, 1.0);
}