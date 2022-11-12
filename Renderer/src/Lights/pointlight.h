#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <QVector3D>

class PointLight
{
public:
    PointLight(const QVector3D& position = QVector3D(0.0f, 0.0f, 0.0f),
               const QVector3D& color = QVector3D(0.0f, 0.0f, 0.0f),
               float intensity = 1.0f);

    QVector3D Position;
    QVector3D Color;
    float Intensity;
};

#endif // POINTLIGHT_H
