#include "modelnode.h"

ModelNode::ModelNode(const std::shared_ptr<Mesh>& pMesh)
{
    this->pMesh = pMesh;
}

ModelNode::ModelNode(const std::shared_ptr<Mesh>& pMesh, const Material& material)
{
    this->pMesh = pMesh;
    this->mMaterial = material;
}


