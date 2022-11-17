#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform bool useDepthMap;
uniform bool useDiffuseTexture;
uniform bool useSpecularTexture;
uniform bool useNormalTexture;
uniform bool useHeightTexture;

uniform vec3 uCameraPos;
uniform vec3 uLightPos; 
uniform vec3 uDirLightDir;
uniform vec3 uDirLightColor;

uniform sampler2D uDepthMap;
uniform sampler2D uDiffuseTexture0;
uniform sampler2D uSpecularTexture0;
uniform sampler2D uNormalTexture0;
uniform sampler2D uHeightTexture0;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;


vec3 Ambient;
vec3 Diffuse;
vec3 Specular;

vec3 CalcDirLight(vec3 lightDir, vec3 normal, vec3 viewDir);

void main(){
    Ambient  = material.ambient;
    Diffuse  = useDiffuseTexture ? texture(uDiffuseTexture0,TexCoord).rgb : material.diffuse;
    Specular = useSpecularTexture ? texture(uSpecularTexture0,TexCoord).rgb : material.specular;
    
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(uCameraPos - FragPos);
    vec3 lightDir = normalize(-uDirLightDir);

    vec3 result = vec3(0.0, 0.0, 0.0);
    result += CalcDirLight(lightDir, normal, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 lightDir, vec3 normal, vec3 viewDir){
    vec3 ambient = uDirLightColor * Ambient;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = uDirLightColor * diff * Diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = uDirLightColor * spec * Specular;

    return ambient + diffuse + specular;
}