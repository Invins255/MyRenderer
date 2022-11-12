#include "resourcemanager.h"

ResourceManager::~ResourceManager()
{

}

void ResourceManager::loadResources()
{
    loadShader("DepthShader",
        "D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/depthShader.vert",
        "D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/depthShader.frag"
    );    
    loadShader("PhongShader",
        "D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/phongShader.vert",
        "D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/phongShader.frag"
    );
    loadShader("ShadowShader",
        "D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/shadowShader.vert",
        "D:/ProjectFiles/Cpp/Renderer/Renderer/src/Shaders/glsl/shadowShader.frag"
    );

    loadModel(Model::BasicType::CUBE);
    loadModel(Model::BasicType::PLANE);
    loadModel(Model::BasicType::SPHERE);
    loadModel("D:/ProjectFiles/Cpp/Renderer/Renderer/resources/models/obj/nanosuit/nanosuit.obj");
    loadModel("D:/ProjectFiles/Cpp/Renderer/Renderer/resources/models/fbx/Mei/Mei.fbx");


    loadTexture("D:/ProjectFiles/Cpp/Renderer/Renderer/resources/textures/gray.png");

    auto pPlane = getInstance().Models["Plane"];
    pPlane->modelNodes[0]->material.uniform.textureID.insert("uTexture",
                                                             getInstance().Textures["D:/ProjectFiles/Cpp/Renderer/Renderer/resources/textures/gray.png"]->textureId());
    auto pCube = getInstance().Models["Cube"];
    pCube->modelNodes[0]->material.uniform.textureID.insert("uTexture",
                                                            getInstance().Textures["D:/ProjectFiles/Cpp/Renderer/Renderer/resources/textures/gray.png"]->textureId());
    auto pSphere = getInstance().Models["Sphere"];
    pSphere->modelNodes[0]->material.uniform.textureID.insert("uTexture",
                                                             getInstance().Textures["D:/ProjectFiles/Cpp/Renderer/Renderer/resources/textures/gray.png"]->textureId());
    
}

ResourceManager::ResourceManager()
{

}

void ResourceManager::loadModel(Model::BasicType type)
{
    QString name;
    switch (type) {
    case Model::BasicType::CUBE:
    {
        name = "Cube";
        if (Models.count(name) == 0) {
            auto pModel = std::make_shared<Model>("", name);
            Mesh cube(Mesh::BasicMesh::CUBE);
            Material material;
            pModel->modelNodes.push_back(std::make_shared<ModelNode>(cube, material));

            Models.insert(name, pModel);
        }
        break;
    }
    case Model::BasicType::SPHERE:
    {
        name = "Sphere";
        if (Models.count(name) == 0) {
            auto pModel = std::make_shared<Model>("", name);
            Mesh sphere(Mesh::BasicMesh::SPHERE);
            Material material;
            pModel->modelNodes.push_back(std::make_shared<ModelNode>(sphere, material));

            Models.insert(name, pModel);
        }
        break;
    }
    case Model::BasicType::PLANE:
    {
        name = "Plane";
        if (Models.count(name) == 0) {
            auto pModel = std::make_shared<Model>("", name);
            Mesh plane(Mesh::BasicMesh::PLANE);
            Material material;
            pModel->modelNodes.push_back(std::make_shared<ModelNode>(plane, material));

            Models.insert(name, pModel);
        }
        break;
    }        
    case Model::BasicType::QUAD:
    {
        name = "QUAD";
        if (Models.count(name) == 0) {
            auto pModel = std::make_shared<Model>("", name);
            Mesh quad(Mesh::BasicMesh::QUAD);
            Material material;
            pModel->modelNodes.push_back(std::make_shared<ModelNode>(quad, material));

            Models.insert(name, pModel);
        }
        break;
    }    
    default:
        break;
    }

}

void ResourceManager::loadModel(const QString &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
    );

    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::" << importer.GetErrorString();
        return;
    }

    if (Models.count(path) == 0) {
        QString directory = path.mid(0, path.lastIndexOf('/'));
        QString name = path.mid(path.lastIndexOf('/') + 1);

        Models.insert(path, std::make_shared<Model>(directory, name));
    }
    auto pModel = Models[path];

    processNode(scene->mRootNode, scene, *pModel);
    auto mat = scene->mRootNode->mTransformation;
}

void ResourceManager::loadTexture(const QString &path)
{
    auto pTexture = std::make_shared<QOpenGLTexture>(QImage(path));
    Textures.insert(path, pTexture);
}

void ResourceManager::loadShader(const QString &name, const QString &vert, const QString &frag, const QString &geo)
{
    auto pShader = std::make_shared<Shader>(vert, frag, geo);
    Shaders.insert(name, pShader);
}

void ResourceManager::releaseResources()
{
    Models.clear();
    Textures.clear();
    Shaders.clear();
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

    Mesh mMesh(vertices, indices);

    //material
    Material mMaterial;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    float value;
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    mMaterial.uniform.vec3.insert(MATERIAL_AMBIENT, QVector3D(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mMaterial.uniform.vec3.insert(MATERIAL_DIFFUSE, QVector3D(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    mMaterial.uniform.vec3.insert(MATERIAL_SPECULAR, QVector3D(color.r, color.g, color.b));
    material->Get(AI_MATKEY_SHININESS, value);
    mMaterial.uniform.floats.insert(MATERIAL_SHININESS, value / 4.0f);  //数据修正

    //textures
    QVector<int> diffuseTextureIds = loadMaterialTextures(material, aiTextureType_DIFFUSE, model);
    mMaterial.uniform.bools.insert(USE_DIFFUSEMAP, diffuseTextureIds.count() != 0);
    for (int i = 0; i < diffuseTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.uniform.textureID.insert(DIFFUCEMAP + number, diffuseTextureIds[i]);
    }

    QVector<int> specularTextureIds = loadMaterialTextures(material, aiTextureType_SPECULAR, model);
    mMaterial.uniform.bools.insert(USE_SPECULARMAP, specularTextureIds.count() != 0);
    for (int i = 0; i < specularTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.uniform.textureID.insert(SPECULARMAP + number, specularTextureIds[i]);
    }

    QVector<int> normalTextureIds = loadMaterialTextures(material, aiTextureType_NORMALS, model);
    mMaterial.uniform.bools.insert(USE_NORMALMAP, normalTextureIds.count() != 0);
    for (int i = 0; i < normalTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.uniform.textureID.insert(NORMALMAP + number, normalTextureIds[i]);
    }

    QVector<int> heightTextureIds = loadMaterialTextures(material, aiTextureType_HEIGHT, model);
    mMaterial.uniform.bools.insert(USE_HEIGHTMAP, heightTextureIds.count() != 0);
    for (int i = 0; i < heightTextureIds.count(); i++) {
        std::string number = std::to_string(i);
        mMaterial.uniform.textureID.insert(HEIGHTMAP + number, heightTextureIds[i]);
    }
    //TODO：添加更多纹理类型

    model.modelNodes.push_back(std::make_shared<ModelNode>(mMesh, mMaterial));
}

QVector<int> ResourceManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Model& model)
{
    QVector<int> textureIds;
    for (int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        QString path = model.directory() + "/" + QString(str.C_Str());
        //检查该纹理是否已经被加载, 若已经加载，则直接获取id，反之则将其载入
        if (Textures.count(path) == 0) {
            loadTexture(path);
        }
        textureIds.push_back(Textures[path]->textureId());
    }

    return textureIds;
}
