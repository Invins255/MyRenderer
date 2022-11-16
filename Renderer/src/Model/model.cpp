#include "model.h"


Model::Model(const QString& directory, const QString& name, const QVector<ModelNode>& modelNodes) :
    mDirectory(directory), mName(name), mModelNodes(modelNodes)
{

}
