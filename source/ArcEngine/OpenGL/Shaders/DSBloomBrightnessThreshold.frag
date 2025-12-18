#version 460 core

in vec2 TexCoords;

uniform sampler2D gScene;
uniform sampler2D customInput01;
uniform sampler2D customInput02;

layout (location = 0) out vec4 customOutput01;
layout (location = 1) out vec4 customOutput02;

uniform float threshold = 0.5;

void main()
{ 
    vec4 color = texture(gScene, TexCoords);
    
    float brightness = dot(color.rgb, vec3(threshold));

    if(brightness > 1.0)
        customOutput01 = vec4(color.rgb, 1.0);
    else
        customOutput01 = vec4(0.0, 0.0, 0.0, 1.0);
}