#include "resourcemanager.h"

QString ResourceManager::CubeMapInfo::frontPattern = "(front|posz|positivez).(jpg|png)";
QString ResourceManager::CubeMapInfo::backPattern = "(back|negz|negativez).(jpg|png)";
QString ResourceManager::CubeMapInfo::leftPattern = "(left|negx|negativex).(jpg|png)";
QString ResourceManager::CubeMapInfo::rightPattern = "(right|posx|positivex).(jpg|png)";
QString ResourceManager::CubeMapInfo::topPattern = "(top|posy|positivey).(jpg|png)";
QString ResourceManager::CubeMapInfo::bottomPattern = "(bottom|negy|negativey).(jpg|png)";

QString ResourceManager::ShaderInfo::vertexPattern = "(\\w*).(vert)";
QString ResourceManager::ShaderInfo::fragmentPattern = "(\\w*).(frag)";
QString ResourceManager::ShaderInfo::geometryPattern = "(\\w*).(geo)";


ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::loadResources()
{
    
}

void ResourceManager::releaseResources()
{
    models.clear();
    meshes.clear();
    textures.clear();
    shaders.clear();
}

const Model ResourceManager::getModel(Mesh::BasicMesh type)
{
    QString name;
    switch (type)
    {
    case Mesh::BasicMesh::CUBE:
        name = "Cube";
        break;
    case Mesh::BasicMesh::SPHERE:
        name = "Sphere";
        break;
    case Mesh::BasicMesh::PLANE:
        name = "Plane";
        break;
    case Mesh::BasicMesh::QUAD:
        name = "Quad";
        break;
    default:
        break;
    }

    if (models.count(name) == 0) {
        Model model = loadModel(name, type);
        models.insert(name, model);
    }
    return models[name];
}

const Model ResourceManager::getModel(const QString& path)
{
    if (models.count(path) == 0) {
        Model model = loadModel(path);
        models.insert(path, model);
    }
    return models[path];
}

std::shared_ptr<QOpenGLTexture> ResourceManager::getTexture(const QString& path)
{
    if (textures.count(path) == 0) {
        auto texture = loadTexture(path);
        textures.insert(path, texture);
    }
    return textures[path];
}

std::shared_ptr<QOpenGLTexture> ResourceManager::getCubeMap(const QString& path)
{    
    if (textures.count(path) == 0) {
        CubeMapInfo info = findCubeMap(path);
        auto cubeTexture = loadCubeMap(info.front, info.back, info.left, info.right, info.top, info.bottom);
        textures.insert(path, cubeTexture);
    }    
    return textures[path];
}

std::shared_ptr<Shader> ResourceManager::getShader(const QString& path)
{
    if (shaders.count(path) == 0) {
        ShaderInfo info = findShader(path);
        auto shader = loadShader(info.vertex, info.fragment, info.geometry);
        shaders.insert(path, shader);
    }
    return shaders[path];
}

Model ResourceManager::loadModel(const QString& name, Mesh::BasicMesh type)
{
    Model model("", name); 

    //若mesh未创建，创建mesh
    if(meshes.count(name) == 0){
        auto pMesh = loadMesh(type);
        meshes.insert(name, pMesh);
    }

    model.mModelNodes.push_back(ModelNode(meshes[name]));
    
    return model;
}

Model ResourceManager::loadModel(const QString &path)
{
    QString directory = path.mid(0, path.lastIndexOf('/'));
    QString name = path.mid(path.lastIndexOf('/') + 1);
    Model model(directory, name);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
    );

    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::" << importer.GetErrorString();
        return model;
    }

    processNode(scene->mRootNode, scene, model);
    
    return model;
}

std::shared_ptr<Mesh> ResourceManager::loadMesh(Mesh::BasicMesh type)
{
    return std::make_shared<Mesh>(type);
}

std::shared_ptr<Mesh> ResourceManager::loadMesh(const QVector<Vertex> vertices, const QVector<unsigned int> indices)
{    
    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<QOpenGLTexture> ResourceManager::loadTexture(const QString &path)
{    
    return std::make_shared<QOpenGLTexture>(QImage(path));
}

std::shared_ptr<QOpenGLTexture> ResourceManager::loadCubeMap(const QString& front, const QString& back, const QString& left, const QString& right, const QString& top, const QString& bottom)
{    
    QImage posx = QImage(right).convertToFormat(QImage::Format_RGB888);
    QImage posy = QImage(top).convertToFormat(QImage::Format_RGB888);
    QImage posz = QImage(front).convertToFormat(QImage::Format_RGB888);
    QImage negx = QImage(left).convertToFormat(QImage::Format_RGB888);
    QImage negy = QImage(bottom).convertToFormat(QImage::Format_RGB888);
    QImage negz = QImage(back).convertToFormat(QImage::Format_RGB888);

    auto pCubeTexture = std::make_shared<QOpenGLTexture>(QOpenGLTexture::TargetCubeMap);
    pCubeTexture->setSize(posx.width(), posx.height(), posx.depth());
    pCubeTexture->setFormat(QOpenGLTexture::RGBFormat);
    pCubeTexture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);
    
    pCubeTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, 
        QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
        (const void*)posx.bits());
    pCubeTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
        QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
        (const void*)posy.bits());
    pCubeTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
        QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
        (const void*)posz.bits());
    pCubeTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
        QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
        (const void*)negx.bits());
    pCubeTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
        QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
        (const void*)negy.bits());
    pCubeTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
        QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
        (const void*)negz.bits());

    pCubeTexture->generateMipMaps();
    pCubeTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    pCubeTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    pCubeTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    return pCubeTexture;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const QString& vert, const QString& frag, const QString& geo)
{
    return std::make_shared<Shader>(vert, frag, geo);
}

void ResourceManager::processNode(aiNode* node, const aiScene* scene, Model& model)
{
    //处理当前节点
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, model);
    }

    //递归处理子节点
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, model);
    }
}

void ResourceManager::processMesh(aiMesh* mesh, const aiScene* scene, Model& model)
{
    //vertices
    QVector<Vertex> vertices;
    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        //position
        QVector3D vector3;
        vector3.setX(mesh->mVertices[i].x);
        vector3.setY(mesh->mVertices[i].y);
        vector3.setZ(mesh->mVertices[i].z);
        vertex.position = vector3;

        //normal
        if (mesh->HasNormals()) {
            vector3.setX(mesh->mNormals[i].x);
            vector3.setY(mesh->mNormals[i].y);
            vector3.setZ(mesh->mNormals[i].z);
            vertex.normal = vector3;
        }

        //texcoords
        if (mesh->mTextureCoords[0]) {
            QVector2D vector2;
            vector2.setX(mesh->mTextureCoords[0][i].x);
            vector2.setY(mesh->mTextureCoords[0][i].y);
            vertex.texCoords = vector2;
        }
        else {
            vertex.texCoords = QVector2D(0.0f, 0.0f);
        }

        //TODO：添加更多顶点属性

        vertices.push_back(vertex);
    }

    //indices
    QVector<unsigned int> indices;
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    QString path = model.directory() + "/" + model.name() + "/" + QString(mesh->mName.C_Str()) + "_" + QString::number(model.mModelNodes.size());
    if(meshes.count(path) == 0){
        auto mesh = loadMesh(vertices, indices);
        meshes.insert(path, mesh);
    }
    auto pMesh = meshes[path];

    //material
    Material mMaterial;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    float value;
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    mMaterial.addVector(MATERIAL_AMBIENT, QVector3D(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mMaterial.addVector(MATERIAL_DIFFUSE, QVector3D(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    mMaterial.addVector(MATERIAL_SPECULAR, QVector3D(color.r, color.g, color.b));
    material->Get(AI_MATKEY_SHININESS, value);
    mMaterial.addFloat(MATERIAL_SHININESS, value / 4.0f);  //数据修正

    //textures
    QVector<int> diffuseTextureIds = loadMaterialTextures(material, aiTextureType_DIFFUSE, model);
    mMaterial.addBool(USE_DIFFUSEMAP, diffuseTextureIds.count() != 0);
    for (int i = 0; i < diffuseTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.addTexture(DIFFUCEMAP + number, diffuseTextureIds[i]);
    }

    QVector<int> specularTextureIds = loadMaterialTextures(material, aiTextureType_SPECULAR, model);
    mMaterial.addBool(USE_SPECULARMAP, specularTextureIds.count() != 0);
    for (int i = 0; i < specularTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.addTexture(SPECULARMAP + number, specularTextureIds[i]);
    }

    QVector<int> normalTextureIds = loadMaterialTextures(material, aiTextureType_NORMALS, model);
    mMaterial.addBool(USE_NORMALMAP, normalTextureIds.count() != 0);
    for (int i = 0; i < normalTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.addTexture(NORMALMAP + number, normalTextureIds[i]);
    }

    QVector<int> heightTextureIds = loadMaterialTextures(material, aiTextureType_HEIGHT, model);
    mMaterial.addBool(USE_HEIGHTMAP, heightTextureIds.count() != 0);
    for (int i = 0; i < heightTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.addTexture(HEIGHTMAP + number, heightTextureIds[i]);
    }
    //TODO：添加更多纹理类型

    model.mModelNodes.push_back(ModelNode(pMesh, mMaterial));
}

QVector<int> ResourceManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Model& model)
{
    QVector<int> textureIds;
    for (int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        QString path = model.directory() + "/" + QString(str.C_Str());
        //检查该纹理是否已经被加载, 若已经加载，则直接获取id，反之则将其载入
        if (textures.count(path) == 0) {
            auto texture = loadTexture(path);
            textures.insert(path, texture);
        }
        textureIds.push_back(textures[path]->textureId());
    }

    return textureIds;
}

ResourceManager::CubeMapInfo ResourceManager::findCubeMap(const QString& directory)
{
    CubeMapInfo info;

    QDir dir(directory);
    if (!dir.exists()) {
        qDebug() << "ERROE:: Cube Map directory: " << directory << " is not exist!";
        return info;
    }

    QRegExp rx;
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    dir.setFilter(QDir::Files);
    QFileInfoList fileInfoList = dir.entryInfoList();
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(CubeMapInfo::frontPattern);
        
        if (rx.exactMatch(fileName)) {
            info.front = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(CubeMapInfo::backPattern);
        if (rx.exactMatch(fileName)) {
            info.back = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(CubeMapInfo::leftPattern);
        if (rx.exactMatch(fileName)) {
            info.left = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(CubeMapInfo::rightPattern);
        if (rx.exactMatch(fileName)) {
            info.right = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(CubeMapInfo::topPattern);
        if (rx.exactMatch(fileName)) {
            info.top = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(CubeMapInfo::bottomPattern);
        if (rx.exactMatch(fileName)) {
            info.bottom = directory + "/" + fileName;
            break;
        }
    }

    return info;
}

ResourceManager::ShaderInfo ResourceManager::findShader(const QString& directory) {
    ShaderInfo info;

    QDir dir(directory);
    if (!dir.exists()) {
        qDebug() << "ERROE:: Shader directory: " << directory << " is not exist!";
        return info;
    }

    QRegExp rx;
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    dir.setFilter(QDir::Files);
    QFileInfoList fileInfoList = dir.entryInfoList();
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(ShaderInfo::vertexPattern);

        if (rx.exactMatch(fileName)) {
            info.vertex = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(ShaderInfo::fragmentPattern);

        if (rx.exactMatch(fileName)) {
            info.fragment = directory + "/" + fileName;
            break;
        }
    }
    for (int i = 0; i < fileInfoList.count(); i++) {
        QString fileName = fileInfoList[i].fileName();
        rx.setPattern(ShaderInfo::geometryPattern);

        if (rx.exactMatch(fileName)) {
            info.geometry = directory + "/" + fileName;
            break;
        }
    }

    return info;
}