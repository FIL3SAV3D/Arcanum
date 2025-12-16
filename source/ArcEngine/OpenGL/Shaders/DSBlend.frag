#version 460 core

in vec2 TexCoords;

uniform sampler2D customInput01;
uniform sampler2D customInput02;

layout (location = 0) out vec4 customOutput01;
layout (location = 1) out vec4 customOutput02;


void main()
{ 
    vec4 hdrColor = texture(customInput01, TexCoords);      
    vec4 bloomColor = texture(customInput02, TexCoords);
    hdrColor += bloomColor;

    customOutput01 = hdrColor;
}