#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QScopedPointer>
#include <QHash>

class QOpenGLTexture;

class Widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    void initializeTriangle();
    void initializeShaders();
    void doMovement();
    void doRotation();

    void updateAngles(const QPointF &mousePos);
    void updateFov(float angle);

    static const QVector<QVector3D> &positions();
    QOpenGLTexture *initializeTextures(const QString &path);

    QOpenGLBuffer vertexBuffer, indexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLShaderProgram program;

    QScopedPointer<QOpenGLTexture> container;
    QScopedPointer<QOpenGLTexture> face;

    QVector3D cameraPos = {0.0f, 0.0f, -3.0f};
    QVector3D cameraFront = {0.0f, 0.0f, -1.0f};
    const QVector3D cameraUp = {0.0f, 1.0f, 0.0f};

    QHash<int, bool> keys = {
        {Qt::Key_W, false},
        {Qt::Key_A, false},
        {Qt::Key_S, false},
        {Qt::Key_D, false},
    };

    float pitch = 0;
    float yaw = 0;
    float roll = 0;

    float fov = 45.0f;

};
#endif // WIDGET_H
