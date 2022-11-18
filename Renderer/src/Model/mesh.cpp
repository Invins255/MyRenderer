#include "mesh.h"

Mesh::Mesh(const QVector<Vertex> vertices, const QVector<unsigned int> indices)
{
    this->_vertices = vertices;
    this->_indices = indices;

    _hasNormals = false;
    _hasTexcoords = false;

    for(int i=0; i < vertices.count();  i++){
        if(vertices[i].normal != QVector3D(0.0f, 0.0f, 0.0f))
            _hasNormals = true;
        if(vertices[i].texCoords != QVector2D(0.0f, 0.0f))
            _hasTexcoords = true;
    }

    _hasIndices = !indices.empty();
}

Mesh::Mesh(BasicMesh type)
{
    switch (type) {
    case BasicMesh::CUBE:
        initializeCube();
        break;
    case BasicMesh::SPHERE:
        initializeSphere();
        break;
    case BasicMesh::PLANE:
        initializePlane();
        break;
    case BasicMesh::QUAD:
        initializeQuad();
        break;
    default:
        break;
    }
}

void Mesh::initializeCube()
{
    float datas[] ={
        // positions          // normals           // texture coords
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    QVector<Vertex> vertices;
    for(int i = 0; i < 36; i++){
        Vertex v;
        v.position = QVector3D(datas[8*i],datas[8*i+1],datas[8*i+2]);
        v.normal = QVector3D(datas[8*i+3],datas[8*i+4],datas[8*i+5]);
        v.texCoords =QVector2D(datas[8*i+6],datas[8*i+7]);
        vertices.push_back(v);
    }

    this->_vertices = vertices;
    this->_indices = {};
    _hasNormals = true;
    _hasIndices = false;
    _hasTexcoords = true;
}

void Mesh::initializeSphere()
{
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            Vertex v;
            v.position = QVector3D(xPos, yPos, zPos);
            v.normal = QVector3D(xPos, yPos, zPos);
            v.texCoords= QVector2D(xSegment, ySegment);
            vertices.push_back(v);
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    this->_vertices = vertices;
    this->_indices = indices;
    _hasNormals = true;
    _hasIndices = true;
    _hasTexcoords = true;

}

void Mesh::initializePlane()
{
    float datas[] = {
        // Positions            // Normals            // Texture Coords
        10.0f, -0.0f, 10.0f,    0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
        -10.0f, -0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        -10.0f, -0.0f, 10.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

        10.0f, -0.0f, 10.0f,    0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
        10.0f, -0.0f, -10.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f,
        - 10.0f, -0.0f, -10.0f, 0.0f, 1.0f, 0.0f,   0.0f, 10.0f
    };

    QVector<Vertex> vertices;
    for(int i = 0; i < 6; i++){
        Vertex v;
        v.position = QVector3D(datas[8*i],datas[8*i+1],datas[8*i+2]);
        v.normal = QVector3D(datas[8*i+3],datas[8*i+4],datas[8*i+5]);
        v.texCoords =QVector2D(datas[8*i+6],datas[8*i+7]);
        vertices.push_back(v);
    }

    this->_vertices = vertices;
    this->_indices = {};
    _hasNormals = true;
    _hasIndices = false;
    _hasTexcoords = true;
}

void Mesh::initializeQuad()
{
    QVector<Vertex> vertices;
    float datas[] = {
        // positions            // Normals          // texture Coords
        -1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    };

    for(int i = 0; i < 6; i++){
        Vertex v;
        v.position = QVector3D(datas[8*i],datas[8*i+1],datas[8*i+2]);
        v.normal = QVector3D(datas[8*i+3],datas[8*i+4],datas[8*i+5]);
        v.texCoords =QVector2D(datas[8*i+6],datas[8*i+7]);
        vertices.push_back(v);
    }

    this->_vertices = vertices;
    this->_indices = {};
    _hasNormals = false;
    _hasIndices = false;
    _hasTexcoords = true;

}



