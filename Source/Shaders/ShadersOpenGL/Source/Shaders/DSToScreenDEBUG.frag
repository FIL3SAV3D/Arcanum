#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gORM;
uniform sampler2D gDepth;
uniform sampler2D gCombined;

uniform float DebugPosition;
uniform float DebugNormal;
uniform float DebugAlbedo;
uniform float DebugORM;
uniform float DebugDepth;
uniform float DebugCombined;

uniform vec2 DebugDepthRange;

void main()
{ 
    vec4 position = vec4(texture(gPosition, TexCoords).rgb, 1.0);
    vec4 normal = vec4(texture(gNormal, TexCoords).rgb, 1.0);
    vec4 albedo = vec4(texture(gAlbedo, TexCoords).rgb, 1.0);
    vec4 orm = vec4(texture(gORM, TexCoords).rgb, 1.0);
    float depth = texture(gDepth, TexCoords).r;
    vec4 combined = vec4(texture(gCombined, TexCoords));

    depth = (depth - DebugDepthRange.x) / DebugDepthRange.y;

    FragColor += mix(vec4(0), position, DebugPosition);
    FragColor += mix(vec4(0), normal, DebugNormal);
    FragColor += mix(vec4(0), albedo, DebugAlbedo);
    FragColor += mix(vec4(0), orm, DebugORM);
    FragColor += mix(vec4(0), vec4(vec3(depth), 1.0), DebugDepth);
    FragColor += mix(vec4(0), combined, DebugCombined);
}