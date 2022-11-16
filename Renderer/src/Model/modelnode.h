#ifndef MODELNODE_H
#define MODELNODE_H

#include <QVector>
#include <memory>
#include "Material/material.h"
#include "Model/mesh.h"

class ModelNode
{
public:
    ModelNode(const std::shared_ptr<Mesh>& pMesh);
    ModelNode(const std::shared_ptr<Mesh>& pMesh, const Material& material);

    std::shared_ptr<Mesh> pMesh;
    Material mMaterial;
};

#endif // MODELNODE_H
