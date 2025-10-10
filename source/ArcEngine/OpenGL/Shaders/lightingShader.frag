#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;
  
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;

// Lights
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

float near = 0.1; 
float far  = 1000.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir)
{
    vec3 lightDir = normalize(-_light.direction);

    // diffuse shading
    float diff = max(dot(_normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = _light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = _light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = _light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight _light, vec3 _normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(_light.position - fragPos);
    // diffuse shading
    float diff = max(dot(_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(_light.position - fragPos);
    float attenuation = 1.0 / (_light.constant + _light.linear * distance + _light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = _light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = _light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = _light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    float alpha = texture(material.diffuse, TexCoords).a;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, alpha);

    //FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / 500), 1.0);
}