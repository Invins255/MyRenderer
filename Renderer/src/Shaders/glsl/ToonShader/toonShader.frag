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

vec3 CalcDirLight(vec3 lightDir, vec3 normal, vec3 viewDir);
float lerp(float x1, float x2, float weight) {return x1 * (1.0 - weight) + x2 * weight;}

vec3 Ambient;
vec3 Diffuse;
vec3 Specular;

//色阶数量
const int STEP = 3; 
const float effect = 1;

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
    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    diff = (diff + 1.0) / 2.0;
    //diff在[0, 1]内平滑插值
    diff = smoothstep(0.0, 1.0, diff);
    //离散处理diff至各个色阶
    float toonDiff = floor(diff * (STEP - 0.01)) / (float)STEP;
    diff = lerp(diff, toonDiff, effect);
    vec3 diffuse = uDirLightColor * diff * Diffuse;

    //specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    spec = smoothstep(0.0, 1.0, spec);
    //离散处理spec
    float toonSpec = floor(spec * 2) / 2.0;
    spec = lerp(spec, toonSpec, effect);
    vec3 specular = uDirLightColor * spec * Specular;

    //rim
    float ndv = max(dot(normal, viewDir), 0.0);
    float ndl = max(dot(normal, lightDir), 0.0);
    //限制边缘光大小
    ndl = pow(ndl, 4);
    float rim = (1.0 - ndv) * ndl;   
    rim = smoothstep(0.0, 1.0, rim);
    float toonRim = floor(rim * 2) / 2.0;
    rim = lerp(rim, toonRim, effect);
    vec3 rimColor = uDirLightColor * rim * vec3(0.5);

    return diffuse + specular + rimColor;
}
