#version 460 core

in vec2 TexCoords;

uniform sampler2D customInput01;
uniform sampler2D customInput02;

layout (location = 0) out vec4 customOutput01;
layout (location = 1) out vec4 customOutput02;


void main()
{ 
    vec4 color = texture(customInput01, TexCoords);
    customOutput01 = color;
    
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));


    if(brightness > 1.0)
        customOutput02 = vec4(color.rgb, 1.0);
    else
        customOutput02 = vec4(0.0, 0.0, 0.0, 1.0);
}