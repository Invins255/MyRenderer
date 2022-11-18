#include "modelrenderer.h"

ModelRenderer::ModelRenderer(const Model& model):
    mModel(model)
{
    initializeOpenGLFunctions();

    VAO = QVector<GLuint>(model.mModelNodes.count(), 0);
    VBO = QVector<GLuint>(model.mModelNodes.count(), 0);
    EBO = QVector<GLuint>(model.mModelNodes.count(), 0);
}

void ModelRenderer::setRenderMode(GLenum mode)
{
    renderMode = mode;
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
        projection.perspective(camera.Zoom,(float)(camera.width)/(float)(camera.height),camera.NearPlane,camera.FarPlane);
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
    for (int i = 0; i < mModel.mModelNodes.count(); i++) {
        mModel.mModelNodes[i].mMaterial.setTexture(DEPTHMAP, depthMapID);
    }
}

void ModelRenderer::draw()
{
    //在进行draw操作之前，需要对shader，lights，camera进行bind操作，此时已经完成对shader的初次参数传递
    bindTransform();

    for (int i = 0; i < mModel.mModelNodes.count(); i++) {
        mModel.mModelNodes[i].mMaterial.setBool(USE_DEPTHMAP, receiveShadow);
    }
    
    pShader->bind();
    for (int i = 0; i < mModel.mModelNodes.count(); i++) {
        bindMaterial(mModel.mModelNodes[i].mMaterial);

        if (VAO[i] == 0)
            glGenVertexArrays(1, &(VAO[i]));
        if (VBO[i] == 0)
            glGenBuffers(1, &(VBO[i]));
        if (EBO[i] == 0)
            glGenBuffers(1, &(EBO[i]));

        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER,
            mModel.mModelNodes[i].pMesh->vertices().size() * sizeof(Vertex),
            &(mModel.mModelNodes[i].pMesh->vertices()[0]),
            GL_STATIC_DRAW);

        if (mModel.mModelNodes[i].pMesh->hasIndices()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                mModel.mModelNodes[i].pMesh->indices().size() * sizeof(unsigned int),
                &(mModel.mModelNodes[i].pMesh->indices()[0]),
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

        glBindVertexArray(VAO[i]);
        if (mModel.mModelNodes[i].pMesh->hasIndices()) {
            glDrawElements(renderMode,
                static_cast<unsigned int>(mModel.mModelNodes[i].pMesh->indices().size()),
                GL_UNSIGNED_INT,
                0);
        }
        else {
            glDrawArrays(renderMode,
                0,
                mModel.mModelNodes[i].pMesh->vertices().size());
        }
        glBindVertexArray(0);
    }
    pShader->release();
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

void ModelRenderer::bindMaterial(Material& material)
{
    //数值绑定
    for (auto iter = material.uniform.ints.begin(); iter != material.uniform.ints.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.floats.begin(); iter != material.uniform.floats.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.bools.begin(); iter != material.uniform.bools.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), (int)iter.value());
    }
    for (auto iter = material.uniform.vec2.begin(); iter != material.uniform.vec2.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.vec3.begin(); iter != material.uniform.vec3.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.vec4.begin(); iter != material.uniform.vec4.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.matrix3.begin(); iter != material.uniform.matrix3.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }
    for (auto iter = material.uniform.matrix4.begin(); iter != material.uniform.matrix4.end(); iter++) {
        pShader->setUniformValue(iter.key().c_str(), iter.value());
    }

    //纹理绑定
    int index = 0;
    for (auto iter = material.uniform.textureID.begin(); iter != material.uniform.textureID.end(); iter++) {
        glActiveTexture(GL_TEXTURE0 + index);

        if (iter.key() == DEPTHMAP) {
            glBindTexture(GL_TEXTURE_2D, iter.value());

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
        else if (iter.key() == CUBEMAP) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, iter.value());
        }
        else {
            glBindTexture(GL_TEXTURE_2D, iter.value());
        }

        pShader->setUniformValue(iter.key().c_str(), index);
        index++;
    }
}



