#include "openglwidget.h"
#include <QVector>

OpenGLWidget::OpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    //设置OpenGL版本信息
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,5);
    setFormat(format);

    //设置为单击获得焦点
    setFocusPolicy(Qt::ClickFocus);
}

OpenGLWidget::~OpenGLWidget()
{
    ResourceManager::getInstance().releaseResources();
}

void OpenGLWidget::addModelRenderer(std::shared_ptr<ModelRenderer> &modelRenderer)
{
    modelRenderers.push_back(modelRenderer);
}

void OpenGLWidget::addPointLight(const PointLight &pointLight)
{
    pointLights.push_back(pointLight);
}

void OpenGLWidget::initialize()
{
    initializeOpenGLFunctions();
    printContextInfomation();

    //组件初始化
    initializeCamera();
    initializeLights();
    initializeFBO();

    loadResources();
}

void OpenGLWidget::initializeFBO()
{
    depthFBO = std::make_unique<QOpenGLFramebufferObject>(
                directionalLight.resolution,
                directionalLight.resolution,
                QOpenGLFramebufferObject::Depth
                );
}

void OpenGLWidget::initializeCamera()
{
    camera = std::make_unique<OrbitCamera>(
                QVector3D(0.0f, 20.0f, 20.0f),
                QVector3D(0.0f, 10.0f, 0.0f),
                QVector3D(0.0f, 1.0f, 0.0f)
                );
}

void OpenGLWidget::initializeLights()
{
    directionalLight = DirectionalLight(
                QVector3D(5.0f, 8.0f, 10.0f),
                QVector3D(0.0f, 0.0f, 0.0f),
                QVector3D(1.0f, 1.0f, 1.0f),
                1.0f
                );
    pointLights = QVector<PointLight>();
}

void OpenGLWidget::printContextInfomation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
    }
#undef CASE

    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void OpenGLWidget::loadResources(){
    ResourceManager::getInstance().loadResources();  

    //天空盒渲染器
    auto cubeMap = ResourceManager::getInstance().getCubeMap("D:/ProjectFiles/Cpp/Renderer/Renderer/resources/textures/skybox/Space");
    skyboxRenderer = std::make_unique<ModelRenderer>(ResourceManager::getInstance().getModel(Mesh::BasicMesh::CUBE));
    skyboxRenderer->pShaderInScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/Skybox");
    skyboxRenderer->model().mModelNodes[0].mMaterial.addTexture(CUBEMAP, cubeMap->textureId());
    
    auto texture = ResourceManager::getInstance().getTexture("D:/ProjectFiles/Cpp/Renderer/Renderer/resources/textures/gray.png");

    auto pPlaneRenderer = std::make_shared<ModelRenderer>(ResourceManager::getInstance().getModel(Mesh::BasicMesh::PLANE));
    pPlaneRenderer->transform().translate(QVector3D(0.0f, -1.0f, 0.0f));
    pPlaneRenderer->transform().scale(10.0f);
    pPlaneRenderer->pShaderInScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/ShadowShader");
    pPlaneRenderer->pShaderInDepthScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/DepthShader");
    pPlaneRenderer->model().mModelNodes[0].mMaterial.addTexture("uTexture", texture->textureId());
    addModelRenderer(pPlaneRenderer);
    
    
    auto pCubeRenderer = std::make_shared<ModelRenderer>(ResourceManager::getInstance().getModel(Mesh::BasicMesh::CUBE));
    pCubeRenderer->transform().translate(QVector3D(1.5f, 0.5f, 0.0f));
    pCubeRenderer->pShaderInScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/ShadowShader");
    pCubeRenderer->pShaderInDepthScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/DepthShader");
    pCubeRenderer->model().mModelNodes[0].mMaterial.addTexture("uTexture", texture->textureId());
    addModelRenderer(pCubeRenderer);
    
    
    auto pSphereRenderer = std::make_shared<ModelRenderer>(ResourceManager::getInstance().getModel(Mesh::BasicMesh::SPHERE));
    pSphereRenderer->transform().translate(QVector3D(-1.5f, 0.5f, 0.0f));
    pSphereRenderer->setRenderMode(GL_TRIANGLE_STRIP);
    pSphereRenderer->pShaderInScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/ShadowShader");
    pSphereRenderer->pShaderInDepthScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/DepthShader");
    pSphereRenderer->model().mModelNodes[0].mMaterial.addTexture("uTexture", texture->textureId());
    addModelRenderer(pSphereRenderer);
     
    
    auto pModelRenderer = std::make_shared<ModelRenderer>(ResourceManager::getInstance().getModel("D:/ProjectFiles/Cpp/Renderer/Renderer/resources/models/obj/nanosuit/nanosuit.obj"));
    pModelRenderer->pShaderInScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/PhongShader");
    pModelRenderer->pShaderInDepthScene = ResourceManager::getInstance().getShader("D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/DepthShader");
    pModelRenderer->transform().translate(QVector3D(0.0f, 0.0f, -5.0f));
    addModelRenderer(pModelRenderer);

}

void OpenGLWidget::initializeGL()
{
    initialize();
}

void OpenGLWidget::paintGL()
{
    QTime currentFrame = QTime::currentTime();
    deltaTime = lastFrame.msecsTo(currentFrame);
    lastFrame = currentFrame;
    camera->setDeltaTime((float)deltaTime / 1000.0f);

    camera->control();

    renderDepthScene();
    renderScene();
    renderSkybox();

    Input::getInstance().reset();

    update();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWidget::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width() * 4.0 / 3.0, height() * 4.0 / 3.0);
    glClearColor(bgColor.x(), bgColor.y(), bgColor.z(), bgColor.w());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < modelRenderers.count(); i++){
        if(modelRenderers[i]->isVisible){
            modelRenderers[i]->bindShader(modelRenderers[i]->pShaderInScene);
            modelRenderers[i]->bindCamera(*camera);
            modelRenderers[i]->bindLight(directionalLight);
            modelRenderers[i]->bindDepthMap(depthFBO->texture());
            modelRenderers[i]->draw();
        }
    }

    glDisable(GL_DEPTH_TEST);
}

void OpenGLWidget::renderDepthScene()
{
    glViewport(0, 0, directionalLight.resolution, directionalLight.resolution);
    depthFBO->bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < modelRenderers.count(); i++){
            if(modelRenderers[i]->isVisible){
                if(modelRenderers[i]->shadowCast){
                    modelRenderers[i]->bindShader(modelRenderers[i]->pShaderInDepthScene);
                    modelRenderers[i]->bindLight(directionalLight);
                    modelRenderers[i]->draw();
                }
            }
        }

        /*
        QImage image = depthFBO->toImage();
        image.save("D:/ProjectFiles/Qt/Renderer/resources/depthMap.png");
        */

        glDisable(GL_DEPTH_TEST);
    depthFBO->release();
}

void OpenGLWidget::renderSkybox()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    skyboxRenderer->bindShader(skyboxRenderer->pShaderInScene);
    skyboxRenderer->bindCamera(*camera);
    skyboxRenderer->draw();
    glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    Input::getInstance().wheelAngleDelta = event->angleDelta();
}
void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    Input::getInstance().leftButtonPressed = event->button() == Qt::LeftButton;
    Input::getInstance().middleButtonPressed = event->button() == Qt::MiddleButton;
    Input::getInstance().rightButtonPressed = event->button() == Qt::RightButton;

    Input::getInstance().lastMousePos = event->position();
    Input::getInstance().currentMousePos = event->position();
}
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Input::getInstance().isMouseMoving = true;
    Input::getInstance().lastMousePos = Input::getInstance().currentMousePos;
    Input::getInstance().currentMousePos = event->position();
}
void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Input::getInstance().leftButtonPressed = event->button() == Qt::LeftButton;
    Input::getInstance().middleButtonPressed = event->button() == Qt::MiddleButton;
    Input::getInstance().rightButtonPressed = event->button() == Qt::RightButton;

    Input::getInstance().lastMousePos = QPointF(0.0f, 0.0f);
    Input::getInstance().currentMousePos = QPointF(0.0f, 0.0f);
}
void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        Input::getInstance().keys[key] = true;
}
void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        Input::getInstance().keys[key] = false;
}



