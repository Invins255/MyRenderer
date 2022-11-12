#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


uniform vec3 uCameraPos;
uniform vec3 uLightPos; 
uniform vec3 uDirLightDir;
uniform vec3 uDirLightColor;
uniform sampler2D uTexture;

void main()
{
    // ambient
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * uDirLightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-uDirLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uDirLightColor;

    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(uCameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uDirLightColor; 
            
    vec3 result = (ambient + diffuse + specular) * texture(uTexture, TexCoord).rgb;
    FragColor = vec4(result, 1.0);
} 