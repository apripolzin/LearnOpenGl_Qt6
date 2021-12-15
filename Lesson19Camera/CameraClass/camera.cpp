#include "camera.h"

#include <QDateTime>

Camera::Camera()
{
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    yaw   += deltaYaw;
    pitch += deltaPitch;

    pitch = qBound(-89.0f, pitch, 89.0f);
}

void Camera::zoom(float deltaAngle)
{
    fov -= deltaAngle;
    fov = qBound(1.0f, fov, 45.0f);
}

void Camera::update()
{
    doMovement();
    doRotation();
}

void Camera::setViewport(int w, int h)
{
    width = w;
    height = h;
}

QMatrix4x4 Camera::view()
{
    QMatrix4x4 view;
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}

QMatrix4x4 Camera::projection()
{
    QMatrix4x4 projection;
    projection.perspective(fov, 1.0f * width / height, 0.1f, 100.0f);
    return projection;
}

void Camera::move(MoveDirection direction, bool moving)
{
    moveDirections[direction] = moving;
}

void Camera::doMovement()
{
    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    static qint64 lastTime = 0;

    const qint64 deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    const float cameraSpeed = 0.5f / deltaTime;
    if (moveDirections.value(MoveDirection::Direction_Forward) ) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (moveDirections.value(MoveDirection::Direction_Backward)) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (moveDirections.value(MoveDirection::Direction_Left)) {
        cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized()  * cameraSpeed;
    }
    if (moveDirections.value(MoveDirection::Direction_Right)) {
        cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized()  * cameraSpeed;
    }
}

void Camera::doRotation()
{
    QVector3D front;
    front.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    front.setY(qSin(qDegreesToRadians(pitch)));
    front.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));

    cameraFront = front.normalized();
}
