#include "shader.h"

Shader::Shader(const QString &vertexPath,
               const QString &fragmentPath,
               const QString &geometryPath):
    QOpenGLShaderProgram(nullptr)
{   
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    if(!vertexShader.compileSourceFile(vertexPath)){
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << Qt::endl;
        qDebug() << vertexShader.log() << Qt::endl;
    }
    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    if(!fragmentShader.compileSourceFile(fragmentPath)){
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << Qt::endl;
        qDebug() << fragmentShader.log() << Qt::endl;
    }

    addShader(&vertexShader);
    addShader(&fragmentShader);

    if(!geometryPath.isEmpty()){
        QOpenGLShader geometryShader(QOpenGLShader::Geometry);
        if(!geometryShader.compileSourceFile(geometryPath)){
            qDebug() << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED" << Qt::endl;
            qDebug() << geometryShader.log() << Qt::endl;
        }
        addShader(&geometryShader);
    }

    if(!link()){
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << Qt::endl;
        qDebug() << log() << Qt::endl;
    }
}
