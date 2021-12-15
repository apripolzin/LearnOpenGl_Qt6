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
    void rotate(const QPointF &mousePos);

    void zoom(float angle);
    void setViewport(int w, int h);

    void update();

    QMatrix4x4 view();
    QMatrix4x4 projection();


private:
    void doMovement();
    void doRotation();

    static QMatrix4x4 lookAt(const QVector3D &position
                             , const QVector3D &target
                             , const QVector3D &worldUp
                             );

    QVector3D cameraPos = {0.0f, 0.0f, -3.0f};
    QVector3D cameraFront = {0.0f, 0.0f, -1.0f};
    const QVector3D cameraUp = {0.0f, 1.0f, 0.0f};

    QHash<MoveDirection, bool> moveDirections = {
        {MoveDirection::Direction_Forward, false},
        {MoveDirection::Direction_Backward, false},
        {MoveDirection::Direction_Left, false},
        {MoveDirection::Direction_Right, false},
    };

    float pitch = 0;
    float yaw = 0;
    float roll = 0;

    float fov = 45.0f;

    int width = 640;
    int height = 480;
};

#endif // CAMERA_H
