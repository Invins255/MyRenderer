#ifndef MODELNODE_H
#define MODELNODE_H

#include <QVector>
#include <memory>
#include <QOpenGLFunctions_4_5_Core>
#include "Material/material.h"
#include "Model/mesh.h"
#include "Shaders/shader.h"

class ModelNode : protected QOpenGLFunctions_4_5_Core
{
public:
    ModelNode(const Mesh& mesh, const Material& material);

    Mesh mesh;
    Material material;

    void setRenderMode(GLenum mode){_renderMode = mode;}
    void draw(Shader& shader);

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLenum _renderMode = GL_TRIANGLES;

    void bindGeometry();
    void bindMaterial(Shader& shader);
};

#endif // MODELNODE_H
