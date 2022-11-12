#include "model.h"

Model::Model(const QString& directory, const QString& name):
    _directory(directory), _name(name)
{
    /*具体组装过程交由ResoureManager完成*/
}

void Model::bindDepthMap(GLuint depthMapID)
{
    for(int i = 0; i < modelNodes.count(); i++){
        modelNodes[i]->material.uniform.textureID.insert(DEPTHMAP, depthMapID);
    }
}

void Model::setRenderMode(GLenum mode)
{
    for(int i = 0; i < modelNodes.count(); i++){
        modelNodes[i]->setRenderMode(mode);
    }
}

void Model::draw(Shader &shader)
{
    for(int i = 0; i < modelNodes.count(); i++){
        modelNodes[i]->draw(shader);
    }
}
