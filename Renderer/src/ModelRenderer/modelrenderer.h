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
    ModelRenderer(std::shared_ptr<Model>& model);

    void setRenderMode(GLenum mode){renderMode = mode;}
    QMatrix4x4& transform(){return modelMatrix;}

    void bindShader(std::shared_ptr<Shader>& shader);
    void bindLight(const DirectionalLight& dirLight);
    void bindLight(const QVector<PointLight>& pointLights);
    void bindCamera(const Camera& camera);
    void bindDepthMap(GLuint depthMapID);
    void draw();

    bool isVisible = true;
    bool shadowCast = true;
    std::shared_ptr<Shader> pShaderInScene;
    std::shared_ptr<Shader> pShaderInDepthScene;
private:
    GLenum renderMode = GL_TRIANGLES;

    QMatrix4x4 modelMatrix;

    std::shared_ptr<Model> pModel;
    std::shared_ptr<Shader> pShader;
    
    void bindTransform();
};

#endif // MODELRENDERER_H
