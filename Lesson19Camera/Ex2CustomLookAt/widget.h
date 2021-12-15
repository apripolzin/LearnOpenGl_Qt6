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
    void initializeTriangle();
    void initializeShaders();

    static const QVector<QVector3D> &positions();
    QOpenGLTexture *initializeTextures(const QString &path);

    QOpenGLBuffer vertexBuffer, indexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLShaderProgram program;

    QScopedPointer<QOpenGLTexture> container;
    QScopedPointer<QOpenGLTexture> face;

    Camera camera;
};
#endif // WIDGET_H
