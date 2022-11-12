#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLTexture>
#include <QMap>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model/model.h"
#include "Model/modelnode.h"
#include "Model/mesh.h"
#include "Shaders/shader.h"
#include "Shared/shadernamespace.h"


class ResourceManager
{
public:
    static ResourceManager& getInstance(){
        static ResourceManager instance;
        return instance;
    }
    ~ResourceManager();

    QMap<QString, std::shared_ptr<Model>> Models;
    QMap<QString, std::shared_ptr<QOpenGLTexture>> Textures;
    QMap<QString, std::shared_ptr<Shader>> Shaders;

    void loadResources();

    void loadModel(Model::BasicType type);
    void loadModel(const QString& path);
    void loadTexture(const QString& path);
    
    void loadShader(const QString& name, const QString& vert, const QString& frag, const QString& geo = "");   

    void releaseResources();
private:
    ResourceManager();
    ResourceManager(ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&) = delete;

    void processNode(aiNode* node, const aiScene* scene, Model& model);
    void processMesh(aiMesh* mesh, const aiScene* scene, Model& model);
    QVector<int> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Model& model);
};

#endif // RESOURCEMANAGER_H
