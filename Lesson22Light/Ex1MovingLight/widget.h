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

    void drawCube();
    void drawLamp();

    QOpenGLBuffer cubeBuffer;
    QOpenGLBuffer lampBuffer;
    QOpenGLVertexArrayObject cubeVao;
    QOpenGLVertexArrayObject lampVao;
    QOpenGLShaderProgram cubeProgram;
    QOpenGLShaderProgram lampProgram;

    QVector3D lightPos = {1.2f, 1.0f, 2.0f};

    Camera camera;
};
#endif // WIDGET_H
