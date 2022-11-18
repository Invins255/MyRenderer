#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#include <QOpenGLFunctions_4_5_Core>
#include <memory>
#include "Model/model.h"
#include "Camera/camera.h"
#include "Lights/directionallight.h"
#include "Lights/pointlight.h"
#include "Shaders/shader.h"
#include "Shared/shadernamespace.h"

class ModelRenderer : protected QOpenGLFunctions_4_5_Core
{
public:
    ModelRenderer(const Model& model);

    void setRenderMode(GLenum mode);
    QMatrix4x4& transform(){return modelMatrix;}
    Model& model() { return mModel; }

    void bindShader(std::shared_ptr<Shader>& shader);
    void bindLight(const DirectionalLight& dirLight);
    void bindLight(const QVector<PointLight>& pointLights);
    void bindCamera(const Camera& camera);
    void bindDepthMap(GLuint depthMapID);

    void draw();

    bool isVisible = true;
    bool castShadow = true;
    bool receiveShadow = true;
    std::shared_ptr<Shader> pShaderInScene;
    std::shared_ptr<Shader> pShaderInDepthScene;
private:
    QMatrix4x4 modelMatrix;

    Model mModel;
    std::shared_ptr<Shader> pShader;
    
    void bindTransform();
    void bindMaterial(Material& material);

    QVector<GLuint> VAO;
    QVector<GLuint> VBO;
    QVector<GLuint> EBO;

    GLenum renderMode = GL_TRIANGLES;
};

#endif // MODELRENDERER_H
