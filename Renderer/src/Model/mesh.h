#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

struct Vertex
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
};

class Mesh
{    
public:
    enum class BasicMesh{
        CUBE, SPHERE, PLANE, QUAD
    };

    Mesh() = default;
    Mesh(BasicMesh type);
    Mesh(const QVector<Vertex> vertices, const QVector<unsigned int> indices);
    virtual ~Mesh() = default;

    bool hasNormals() {return _hasNormals;}
    bool hasTexcoords() {return _hasTexcoords;}
    bool hasIndices() {return _hasIndices;}

    const QVector<Vertex> vertices() const {return _vertices;}
    const QVector<unsigned int> indices() const {return _indices;}
protected:
    bool _hasNormals;
    bool _hasTexcoords;
    bool _hasIndices;

    QVector<Vertex> _vertices;
    QVector<unsigned int> _indices;
private:
    void initializeCube();
    void initializeSphere();
    void initializePlane();
    void initializeQuad();
};

#endif // MESH_H
