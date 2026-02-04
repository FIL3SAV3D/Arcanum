#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gScene;

void main()
{ 
    const float gamma = 2.2;
    vec3 sceneColor = texture(gScene, TexCoords).rgb;
  
    // reinhard tone mapping
    vec3 mapped = sceneColor / (sceneColor + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}