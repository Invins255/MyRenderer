#version 450 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube uCubeMap;

void main()
{   
    FragColor = texture(uCubeMap, TexCoords);
}