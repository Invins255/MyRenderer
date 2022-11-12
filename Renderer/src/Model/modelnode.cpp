#include "modelnode.h"

ModelNode::ModelNode(const Mesh &mesh, const Material &material)
{
    initializeOpenGLFunctions();
    this->mesh = mesh;
    this->material = material;
}

void ModelNode::draw(Shader &shader)
{
    shader.bind();

    bindMaterial(shader);
    bindGeometry();

    glBindVertexArray(VAO);
    if(mesh.hasIndices()){
        glDrawElements(_renderMode,
                       static_cast<unsigned int>(mesh.indices().size()),
                       GL_UNSIGNED_INT,
                       0);
    }
    else{
        glDrawArrays(_renderMode,
                     0,
                     mesh.vertices().size());
    }
    glBindVertexArray(0);

    shader.release();
}

void ModelNode::bindGeometry()
{
    if(VAO == 0)
        glGenVertexArrays(1, &VAO);
    if(VBO == 0)
        glGenBuffers(1, &VBO);
    if(EBO == 0)
        glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh.vertices().size() * sizeof(Vertex),
                 &(mesh.vertices()[0]),
                 GL_STATIC_DRAW);

    if(mesh.hasIndices()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     mesh.indices().size() * sizeof(unsigned int),
                     &(mesh.indices()[0]),
                     GL_STATIC_DRAW);
    }
    //position layout(0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //normal layout(1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //texcoords layout(2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

void ModelNode::bindMaterial(Shader& shader)
{
    //数值绑定
    for(auto iter = material.uniform.ints.begin(); iter != material.uniform.ints.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }
    for(auto iter = material.uniform.floats.begin(); iter != material.uniform.floats.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.bools.begin(); iter != material.uniform.bools.end(); iter++) {
        shader.setUniformValue(iter.key().c_str(), (int)iter.value());
    }
    for(auto iter = material.uniform.vec2.begin(); iter != material.uniform.vec2.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }
    for(auto iter = material.uniform.vec3.begin(); iter != material.uniform.vec3.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }
    for(auto iter = material.uniform.vec4.begin(); iter != material.uniform.vec4.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }
    for(auto iter = material.uniform.matrix3.begin(); iter != material.uniform.matrix3.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }
    for(auto iter = material.uniform.matrix4.begin(); iter != material.uniform.matrix4.end(); iter++){
        shader.setUniformValue(iter.key().c_str(), iter.value());
    }

    //纹理绑定
    int index = 0;
    for(auto iter = material.uniform.textureID.begin(); iter != material.uniform.textureID.end(); iter++){
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, iter.value());

        if(iter.key() == "uDepthMap"){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        shader.setUniformValue(iter.key().c_str(), index);
        index++;
    }
}
