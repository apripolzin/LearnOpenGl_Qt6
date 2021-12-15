#include "camera.h"

#include <QDateTime>

Camera::Camera()
{
}

void Camera::rotate(const QPointF &mousePos)
{
    static QPointF lastPos = mousePos;
    QPointF currentPos = mousePos;

    float xoffset = currentPos.x() - lastPos.x();
    float yoffset = -(currentPos.y() - lastPos.y());

    lastPos = currentPos;

    static constexpr float sensitivity = 0.2;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    pitch = qBound(-89.0f, pitch, 89.0f);
}

void Camera::zoom(float angle)
{
    fov -= angle;
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
    return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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

QMatrix4x4 Camera::lookAt(const QVector3D &position, const QVector3D &target, const QVector3D &worldUp)
{
    QVector3D zaxis = (position - target).normalized();
    QVector3D xaxis = (QVector3D::crossProduct(worldUp.normalized(), zaxis));
    QVector3D yaxis = QVector3D::crossProduct(zaxis, xaxis);

    QMatrix4x4 translation;
    translation.setColumn(3, {-position.x(), -position.y(), -position.z(), 1});

    QMatrix4x4 rotation;
    rotation.setRow(0, {xaxis.x(), xaxis.y(), xaxis.z(), 0});
    rotation.setRow(1, {yaxis.x(), yaxis.y(), yaxis.z(), 0});
    rotation.setRow(2, {zaxis.x(), zaxis.y(), zaxis.z(), 0});

    return rotation * translation;
}
