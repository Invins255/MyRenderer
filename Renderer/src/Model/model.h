#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include "Model/mesh.h"
#include "Model/modelnode.h"
#include "Shaders/shader.h"
#include "Shared/shadernamespace.h"

class Model
{
public:
    enum class BasicType{
        CUBE, SPHERE, PLANE, QUAD
    };
 
    Model(const QString& directory, const QString& name);
    Model(Model&) = delete;
    Model& operator=(Model&) = delete;

    QString name() { return _name; }
    QString directory() { return _directory; }

    QVector<std::shared_ptr<ModelNode>> modelNodes;

    void bindDepthMap(GLuint depthMapID);
    void setRenderMode(GLenum mode);
    void draw(Shader& shader);
private:

    QString _name;
    QString _directory;

};

#endif // MODEL_H
