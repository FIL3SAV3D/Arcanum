#version 460 core

in vec2 TexCoords;

uniform sampler2D gScene;
uniform sampler2D customInput01;
uniform sampler2D customInput02;

layout (location = 0) out vec4 customOutput01;
layout (location = 1) out vec4 customOutput02;

uniform float exposure = 0.9;


void main()
{ 
    const float gamma = 2.2;
    vec3 hdrColor = texture(gScene, TexCoords).rgb;
    vec3 bloomColor = texture(customInput01, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending

    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it
    result = pow(result, vec3(1.0 / gamma));

    customOutput01 = vec4(result, 1.0);
}