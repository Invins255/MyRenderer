#include "camera.h"


Camera::Camera(const QVector3D &position,
               const QVector3D &front,
               const QVector3D &up):
    Position(position),
    Front(front.normalized()),
    WorldUp(up.normalized()),
    Right(QVector3D::crossProduct(Front, WorldUp).normalized()),
    Up(QVector3D::crossProduct(Right, Front).normalized())
{

}

OrbitCamera::OrbitCamera(const QVector3D &position, const QVector3D &center, const QVector3D &up):
    Camera(position, (center - position).normalized(), up), Center(center)
{
    updateCameraVectors();
}

void OrbitCamera::control()
{
    if(Input::getInstance().keys[Qt::Key_W]){
        translate(UP);
    }
    if(Input::getInstance().keys[Qt::Key_S]){
        translate(DOWN);
    }
    if(Input::getInstance().keys[Qt::Key_A]){
        translate(LEFT);
    }
    if(Input::getInstance().keys[Qt::Key_D]){
        translate(RIGHT);
    }
    if(Input::getInstance().keys[Qt::Key_Q]){
        translate(BACKWARD);
    }
    if(Input::getInstance().keys[Qt::Key_E]){
        translate(FORWARD);
    }    

    if(Input::getInstance().wheelAngleDelta.y() > 0){
        translate(Camera::RADIUSDECREASE);
    }
    else if(Input::getInstance().wheelAngleDelta.y() < 0){
        translate(Camera::RADIUSINCREASE);
    }

    if(Input::getInstance().leftButtonPressed && Input::getInstance().isMouseMoving){
        float xoffset = Input::getInstance().currentMousePos.x() - Input::getInstance().lastMousePos.x();
        float yoffset = Input::getInstance().lastMousePos.y() - Input::getInstance().currentMousePos.y();

        orbit(xoffset, yoffset);
    }
}

void OrbitCamera::translate(Direction direction)
{
    float velocity = MovementSpeed * deltaTime;

    switch(direction)
    {
    case UP:
        Position += Up * velocity;
        Center += Up * velocity;
        break;
    case DOWN:
        Position -= Up * velocity;
        Center -= Up * velocity;
        break;
    case LEFT:
        Position -= Right * velocity;
        Center -= Right * velocity;
        break;
    case RIGHT:
        Position += Right * velocity;
        Center += Right * velocity;
        break;
    case FORWARD:
        Position += Front * velocity;
        Center += Front * velocity;
        break;
    case BACKWARD:
        Position -= Front * velocity;
        Center -= Front * velocity;
        break;
    case RADIUSINCREASE:
        radius += velocity;
        Position = Center + (Position - Center).normalized() * radius;
        break;
    case RADIUSDECREASE:
        radius -= velocity;
        radius = radius < 0.01 ? 0.01 : radius;
        Position = Center + (Position - Center).normalized() * radius;
        break;
    default:
        break;
    }
    updateCameraVectors();
}

void OrbitCamera::updateCameraVectors()
{
    Front = (Center - Position).normalized();
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();
    Up = QVector3D::crossProduct(Right, Front).normalized();

    QVector3D offset = Position - Center;

    radius = offset.length();
    theta = acos(offset.y() / radius);
    phi = atan2(offset.x(), offset.z());
}


void OrbitCamera::orbit(float xoffset, float yoffset)
{
    xoffset *= -0.01f;
    yoffset *= 0.01f;

    phi += xoffset;
    theta += yoffset;
    theta = std::clamp(theta, qDegreesToRadians(0.5f), qDegreesToRadians(179.5f));

    Position.setX(radius * sin(theta) * sin(phi));
    Position.setY(radius * cos(theta));
    Position.setZ(radius * sin(theta) * cos(phi));

    Position += Center;

    updateCameraVectors();
}

FPSCamera::FPSCamera(const QVector3D &position, const QVector3D &front, const QVector3D &up):
    Camera(position, front, up)
{
    Front = front.normalized();
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();
    Up = QVector3D::crossProduct(Right, Front).normalized();
}

void FPSCamera::control()
{
    if(Input::getInstance().keys[Qt::Key_W]){
        translate(UP);
    }
    if(Input::getInstance().keys[Qt::Key_S]){
        translate(DOWN);
    }
    if(Input::getInstance().keys[Qt::Key_A]){
        translate(LEFT);
    }
    if(Input::getInstance().keys[Qt::Key_D]){
        translate(RIGHT);
    }
    if(Input::getInstance().keys[Qt::Key_Q]){
        translate(BACKWARD);
    }
    if(Input::getInstance().keys[Qt::Key_E]){
        translate(FORWARD);
    }


    if(Input::getInstance().keys[Qt::Key_I]){
        rotate(0.0f, 1.0f, 0.0f);
    }
    if(Input::getInstance().keys[Qt::Key_K]){
        rotate(0.0f, -1.0f, 0.0f);
    }
    if(Input::getInstance().keys[Qt::Key_J]){
        rotate(-1.0f, 0.0f, 0.0f);
    }
    if(Input::getInstance().keys[Qt::Key_L]){
        rotate(1.0f, 0.0f, 0.0f);
    }
    if(Input::getInstance().keys[Qt::Key_U]){
        rotate(0.0f, 0.0f, -1.0f);
    }
    if(Input::getInstance().keys[Qt::Key_O]){
        rotate(0.0f, 0.0f, 1.0f);
    }

    if(Input::getInstance().leftButtonPressed && Input::getInstance().isMouseMoving){
        float xoffset = Input::getInstance().currentMousePos.x() - Input::getInstance().lastMousePos.x();
        float yoffset = Input::getInstance().lastMousePos.y() - Input::getInstance().currentMousePos.y();
        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        rotate(xoffset, yoffset, 0.0f);
    }
    if(Input::getInstance().rightButtonPressed && Input::getInstance().isMouseMoving){
        float xoffset = Input::getInstance().currentMousePos.x() - Input::getInstance().lastMousePos.x();
        xoffset *= Sensitivity;

        rotate(0.0f, 0.0f, xoffset);
    }
}

void FPSCamera::translate(Direction direction)
{
    float velocity = MovementSpeed * deltaTime;

    switch(direction)
    {
    case UP:
        Position += Up * velocity;
        break;
    case DOWN:
        Position -= Up * velocity;
        break;
    case LEFT:
        Position -= Right * velocity;
        break;
    case RIGHT:
        Position += Right * velocity;
        break;
    case FORWARD:
        Position += Front * velocity;
        break;
    case BACKWARD:
        Position -= Front * velocity;
        break;
    default:
        break;
    }
}

void FPSCamera::rotate(float yaw, float pitch, float roll)
{
    QMatrix4x4 rx, ry, rz;
    rx.rotate(pitch, Right);
    ry.rotate(yaw, -Up);            //对Up取负可以正确旋转，否则yaw会出现反向
    rz.rotate(roll, Front);
    QMatrix4x4 r = ry * rz * rx;

    Up = (r * QVector4D(Up, 0.0f)).toVector3D().normalized();
    Front = (r * QVector4D(Front, 0.0f)).toVector3D().normalized();
    Right = (r * QVector4D(Right, 0.0f)).toVector3D().normalized();
}
