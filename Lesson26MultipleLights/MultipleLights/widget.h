#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QScopedPointer>

#include "camera.h"

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
    void initializeCubeGeometry();
    void initializeLampGeometry();
    void initializeShaders();
    QOpenGLTexture *initializeTexture(const QString &path);


    void drawCube(const QVector3D &position, float angle);
    void drawLamp(const QVector3D &position);

    QOpenGLBuffer cubeBuffer;
    QOpenGLBuffer lampBuffer;
    QOpenGLVertexArrayObject cubeVao;
    QOpenGLVertexArrayObject lampVao;
    QOpenGLShaderProgram cubeProgram;
    QOpenGLShaderProgram lampProgram;
    QOpenGLTexture *container = nullptr;
    QOpenGLTexture *container_specular = nullptr;

    const QList<QVector3D> lightPositions = {
        { 0.7f,  0.2f,  2.0f},
        { 2.3f, -3.3f, -4.0f},
        {-4.0f,  2.0f, -12.0f},
        { 0.0f,  0.0f, -3.0f}
    };

    Camera camera;
};
#endif // WIDGET_H
