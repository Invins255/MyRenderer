#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include "Model/mesh.h"
#include "Model/modelnode.h"

class Model
{
public:
    QVector<ModelNode> mModelNodes;    
    Model(const QString& directory = "", const QString& name = "", const QVector<ModelNode>& modelNodes = QVector<ModelNode>());
    
    const QString name() const { return mName; }
    const QString directory() const { return mDirectory; }
    

private:    
    QString mName;
    QString mDirectory;
};

#endif // MODEL_H
