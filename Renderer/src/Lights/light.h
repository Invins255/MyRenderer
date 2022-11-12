#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light
{
public:
    Light();
    Light(const QVector3D& position,
          const QVector3D& color
          );

    QVector3D Position;
    QVector3D Color;
};

#endif // LIGHT_H
