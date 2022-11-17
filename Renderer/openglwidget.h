#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H


#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShader>
#include <QMouseEvent>
#include <QTime>
#include <memory>
#include <QOpenGLFramebufferObject>

#include "src/Camera/camera.h"
#include "src/Lights/pointlight.h"
#include "src/Lights/directionallight.h"
#include "src/ModelRenderer/modelrenderer.h"
#include "src/Shared/input.h"
#include "src/ResourceManager/resourcemanager.h"
#include "src/Shared/shadernamespace.h"

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    void addModelRenderer(std::shared_ptr<ModelRenderer>& modelRenderer);
    void addPointLight(const PointLight& pointLight);

private:
    QVector4D bgColor = QVector4D(0.05f, 0.05f, 0.05f, 1.0f);

    //帧率计算(单位/毫秒)
    float deltaTime = 0.0f;
    QTime lastFrame;
    //相机
    std::unique_ptr<Camera> camera;
    //光源
    DirectionalLight directionalLight;
    QVector<PointLight> pointLights;
    //帧缓冲区
    std::unique_ptr<QOpenGLFramebufferObject> depthFBO;
    //模型渲染器组
    std::unique_ptr<ModelRenderer> skyboxRenderer;
    QVector<std::shared_ptr<ModelRenderer>> modelRenderers;

    void initialize();
    void initializeFBO();
    void initializeCamera();
    void initializeLights();
    void printContextInfomation();
    void loadResources();
    void renderScene();
    void renderDepthScene();
    void renderSkybox();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w,int h) override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // OPENGLWIDGET_H
