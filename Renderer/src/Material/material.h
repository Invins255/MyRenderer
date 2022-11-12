#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <memory>


struct Uniform{
    QMap<std::string, int> ints;
    QMap<std::string, float> floats;
    QMap<std::string, bool> bools;
    QMap<std::string, QVector2D> vec2;
    QMap<std::string, QVector3D> vec3;
    QMap<std::string, QVector4D> vec4;
    QMap<std::string, QMatrix3x3> matrix3;
    QMap<std::string, QMatrix4x4> matrix4;
    QMap<std::string, int> textureID;
};

class Material
{
public:
    Material();
    ~Material() = default;

    Uniform uniform;
private:

};

#endif // MATERIAL_H
