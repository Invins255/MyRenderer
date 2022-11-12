#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;


uniform vec3 uCameraPos;
uniform vec3 uLightPos; 
uniform vec3 uDirLightDir;
uniform vec3 uDirLightColor;
uniform sampler2D uTexture;
uniform sampler2D uDepthMap;

float ShadowCalculation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(uDepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

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
            
    // shadow
    float shadow = ShadowCalculation(FragPosLightSpace);

    vec3 result = (ambient +(1.0 - shadow) * (diffuse + specular)) * texture(uTexture, TexCoord).rgb;
    FragColor = vec4(result, 1.0);
} 