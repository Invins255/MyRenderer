#version 450 core
layout (location = 0) in vec3 aPosition;

out vec3 TexCoords;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
    mat4 view = mat4(mat3(uViewMatrix));

    TexCoords = aPosition;
    vec4 pos = uProjectionMatrix * view * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}  