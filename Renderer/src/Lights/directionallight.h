#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <QVector3D>
#include <QMatrix4x4>


class DirectionalLight
{
public:
    DirectionalLight(const QVector3D& begin = QVector3D(10.0f, 10.0f, 10.0f),
                     const QVector3D& end = QVector3D(0.0f, 0.0f, 0.0f),
                     const QVector3D& color = QVector3D(1.0f, 1.0f, 1.0f),
                     float intensity = 1.0f);

    QVector3D Position;
    QVector3D Direction;
    QVector3D Color;
    float Intensity;

    //阴影属性
    float width = 50.0f;
    float height = 50.0f;
    int resolution = 1024;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    const QMatrix4x4 CalcLightSpaceMatrix() const;

private:

};

#endif // DIRECTIONALLIGHT_H
