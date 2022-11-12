#include "directionallight.h"

DirectionalLight::DirectionalLight(const QVector3D &begin, const QVector3D &end, const QVector3D &color, float intensity):
    Position(begin), Direction((end - begin).normalized()), Color(color), Intensity(intensity)
{

}

const QMatrix4x4 DirectionalLight::CalcLightSpaceMatrix() const
{
    QVector3D lightRight = QVector3D::crossProduct(Direction, QVector3D(0.0f, 1.0f, 0.0f)).normalized();
    QVector3D lightUp = QVector3D::crossProduct(lightRight, Direction).normalized();

    QMatrix4x4 lightView, lightProjection;
    lightProjection.ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, nearPlane, farPlane);
    lightView.lookAt(Position, Position + Direction, lightUp);

    return lightProjection * lightView;
}
