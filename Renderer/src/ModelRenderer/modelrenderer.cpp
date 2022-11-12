#include "modelrenderer.h"

ModelRenderer::ModelRenderer(std::shared_ptr<Model> &model):
    pModel(model)
{
    initializeOpenGLFunctions();
}

void ModelRenderer::bindShader(std::shared_ptr<Shader> &shader)
{
    pShader = shader;
}

void ModelRenderer::bindLight(const DirectionalLight &dirLight)
{
    if(pShader){
        pShader->bind();
        pShader->setUniformValue(DIRLIGHT_DIRECTION, dirLight.Direction);
        pShader->setUniformValue(DIRLIGHT_COLOR, dirLight.Color);
        pShader->setUniformValue(DIRLIGHT_INTENSITY, dirLight.Intensity);
        pShader->setUniformValue(LIGHTSPACE_MATRIX, dirLight.CalcLightSpaceMatrix());
    }
    else{
        qDebug()<<"ERROR::There is no shader before binding light!";
    }
}

void ModelRenderer::bindLight(const QVector<PointLight> &pointLights)
{

}

void ModelRenderer::bindCamera(const Camera &camera)
{
    if(pShader){
        pShader->bind();

        QMatrix4x4 projection;
        //TODO：这里将长宽比固定为1:1，当OpenGlWidget长宽比改变时可能导致显示的错误，需要进行适配处理
        projection.perspective(camera.Zoom,(float)(600)/(float)(600),camera.NearPlane,camera.FarPlane);
        QMatrix4x4 view;
        view = camera.getViewMatrix();

        pShader->setUniformValue(PROJECTION_MATRIX, projection);
        pShader->setUniformValue(VIEW_MATRIX, view);
        pShader->setUniformValue(CAMERA_POSITION, camera.Position);
    }
    else{
        qDebug()<<"ERROR::There is no shader before binding camera!";
    }}

void ModelRenderer::bindDepthMap(GLuint depthMapID)
{
    pModel->bindDepthMap(depthMapID);
}

void ModelRenderer::draw()
{
    //在进行draw操作之前，需要对shader，lights，camera进行bind操作，此时已经完成对shader的初次参数传递
    bindTransform();

    pModel->setRenderMode(renderMode);
    pModel->draw(*pShader);
}

void ModelRenderer::bindTransform()
{
    if(pShader){
        pShader->bind();
        pShader->setUniformValue(MODEL_MATRIX, modelMatrix);
    }
    else{
        qDebug()<<"ERROR::There is no shader before binding model Matrix!";
    }
}

