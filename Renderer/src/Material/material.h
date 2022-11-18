#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <memory>

typedef unsigned int GLuint;

struct Uniform{
    QMap<std::string, int> ints;
    QMap<std::string, float> floats;
    QMap<std::string, bool> bools;
    QMap<std::string, QVector2D> vec2;
    QMap<std::string, QVector3D> vec3;
    QMap<std::string, QVector4D> vec4;
    QMap<std::string, QMatrix3x3> matrix3;
    QMap<std::string, QMatrix4x4> matrix4;
    QMap<std::string, GLuint> textureID;
};

class Material
{
public:
    Material();
    ~Material() = default;

    void setInt(const std::string& name, int value) { uniform.ints[name] = value; }
    void setFloat(const std::string& name, float value) { uniform.floats[name] = value; }
    void setBool(const std::string& name, bool value) { uniform.bools[name] = value; }
    void setVector(const std::string& name, const QVector2D& value) { uniform.vec2[name] = value; }
    void setVector(const std::string& name, const QVector3D& value) { uniform.vec3[name] = value; }
    void setVector(const std::string& name, const QVector4D& value) { uniform.vec4[name] = value; }
    void setMatrix(const std::string& name, const QMatrix3x3& value) { uniform.matrix3[name] = value; }
    void setMatrix(const std::string& name, const QMatrix4x4& value) { uniform.matrix4[name] = value; }
    void setTexture(const std::string& name, GLuint value) { uniform.textureID[name] = value; }

    Uniform uniform;
private:

};

#endif // MATERIAL_H
