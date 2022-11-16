#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLTexture>
#include <QMap>
#include <QDir>
#include <QRegExp>
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
private:
    struct CubeMapInfo {
        static QString frontPattern;
        static QString backPattern;
        static QString leftPattern;
        static QString rightPattern;
        static QString topPattern;
        static QString bottomPattern;

        QString front;
        QString back;
        QString left;
        QString right;
        QString top;
        QString bottom;
    };

    struct ShaderInfo {
        static QString vertexPattern;
        static QString fragmentPattern;
        static QString geometryPattern;

        QString vertex;
        QString fragment;
        QString geometry;
    };

public:
    static ResourceManager& getInstance(){
        static ResourceManager instance;
        return instance;
    }
    ~ResourceManager();

    const Model getModel(Mesh::BasicMesh type);
    const Model getModel(const QString& path);
    std::shared_ptr<QOpenGLTexture> getTexture(const QString& path);
    std::shared_ptr<QOpenGLTexture> getCubeMap(const QString& path);
    std::shared_ptr<Shader> getShader(const QString& path);

    void loadResources();
    void releaseResources();

private:
    ResourceManager();
    ResourceManager(ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&) = delete;

    QMap<QString, Model>                            models;
    QMap<QString, std::shared_ptr<Mesh>>            meshes;
    QMap<QString, std::shared_ptr<QOpenGLTexture>>  textures;
    QMap<QString, std::shared_ptr<Shader>>          shaders;

    void processNode(aiNode* node, const aiScene* scene, Model& model);
    void processMesh(aiMesh* mesh, const aiScene* scene, Model& model);
    QVector<int> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Model& model);

    Model loadModel(const QString& name, Mesh::BasicMesh type);
    Model loadModel(const QString& path);
    std::shared_ptr<Mesh> loadMesh(Mesh::BasicMesh type);
    std::shared_ptr<Mesh> loadMesh(const QVector<Vertex> vertices, const QVector<unsigned int> indices);
    std::shared_ptr<QOpenGLTexture> loadTexture(const QString& path);
    std::shared_ptr<QOpenGLTexture> loadCubeMap(const QString& front, const QString& back, const QString& left, const QString& right, const QString& top, const QString& bottom);
    std::shared_ptr<Shader> loadShader(const QString& vert, const QString& frag, const QString& geo = "");

    CubeMapInfo findCubeMap(const QString& directory);
    ShaderInfo findShader(const QString& directory);
};

#endif // RESOURCEMANAGER_H
