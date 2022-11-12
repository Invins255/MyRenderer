#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include "Shared/input.h"

class Camera
{
public:
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD,
        RADIUSINCREASE,
        RADIUSDECREASE
    };

    QVector3D Position;
    QVector3D Front;
    QVector3D WorldUp;
    QVector3D Right;
    QVector3D Up;

    float NearPlane = 0.1f;
    float FarPlane = 100.0f;

    float MovementSpeed = 8.0f;
    float Sensitivity = 0.3f;
    float Zoom = 45.0f;

    void setDeltaTime(float val){deltaTime = val;}

    Camera(const QVector3D& position = QVector3D(0.0f, 0.0f, 3.0f),
           const QVector3D& front = QVector3D(0.0f, 0.0f, -1.0f),
           const QVector3D& up = QVector3D(0.0f, 1.0f, 0.0f));
    virtual ~Camera() = default;
    virtual const QMatrix4x4 getViewMatrix() const = 0;
    virtual void control() = 0;
protected:
    float deltaTime = 0.01f;

};

class OrbitCamera : public Camera{
public:
    QVector3D Center;

    OrbitCamera(const QVector3D& position = QVector3D(0.0f, 0.0f, 3.0f),
                const QVector3D& center = QVector3D(0.0f, 0.0f, 0.0f),
                const QVector3D& up = QVector3D(0.0f, 1.0f, 0.0f));

    const QMatrix4x4 getViewMatrix() const override{
        QMatrix4x4 mat;
        mat.lookAt(Position, Center, Up);
        return mat;
    }

    void control() override;
private:
    float radius;
    float theta;
    float phi;

    void updateCameraVectors();
    void translate(Camera::Direction direction);
    void orbit(float xoffset, float yoffset);
};

class FPSCamera : public Camera{
public:
    FPSCamera(const QVector3D& position = QVector3D(0.0f, 0.0f, 3.0f),
              const QVector3D& front = QVector3D(0.0f, 0.0f, -1.0f),
              const QVector3D& up = QVector3D(0.0f, 1.0f, 0.0f));

    const QMatrix4x4 getViewMatrix() const override{
        QMatrix4x4 mat;
        mat.lookAt(Position, Position + Front, Up);
        return mat;
    }

    void control() override;
private:

    void translate(Camera::Direction direction);
    void rotate(float yaw, float pitch, float roll);
};

#endif // CAMERA_H
