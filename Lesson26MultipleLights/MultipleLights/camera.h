#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QHash>

class Camera
{
public:
    enum class MoveDirection {
        Direction_Forward,
        Direction_Backward,
        Direction_Left,
        Direction_Right
    };

    Camera();

    void move(MoveDirection direction, bool moving = true);
    void rotate(float deltaYaw, float deltaPitch);
    void zoom(float deltaAngle);

    void setViewport(int w, int h);
    void update();

    QMatrix4x4 view();
    QMatrix4x4 projection();

    QVector3D position();
    QVector3D front();

private:
    void doMovement();
    void doRotation();

    QVector3D cameraPos = {0.0f, 0.0f, 3.0f};
    QVector3D cameraFront = {0.0f, 0.0f, -1.0f};
    QVector3D cameraUp = {0.0f, 1.0f, 0.0f};

    QHash<MoveDirection, bool> moveDirections = {
        {MoveDirection::Direction_Forward, false},
        {MoveDirection::Direction_Backward, false},
        {MoveDirection::Direction_Left, false},
        {MoveDirection::Direction_Right, false},
    };

    float pitch = 0.0f;
    float yaw = -90.0f;
    const float roll = 0.0f;

    float fov = 45.0f;

    int width = 640;
    int height = 480;
};

#endif // CAMERA_H
