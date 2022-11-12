#ifndef SHADER_H
#define SHADER_H

#include <QString>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

class Shader: public QOpenGLShaderProgram
{
public:
    Shader(const QString& vertexPath,
           const QString& fragmentPath,
           const QString& geometryPath = "");
};

#endif // SHADER_H
