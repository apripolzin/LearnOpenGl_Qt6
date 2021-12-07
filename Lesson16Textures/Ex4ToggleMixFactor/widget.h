#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QScopedPointer>

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

    void keyPressEvent(QKeyEvent *e) override;

private:
    void initializeTriangle();
    void initializeShaders();
    QOpenGLTexture *initializeTextures(const QString &path);

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLShaderProgram program;

    QScopedPointer<QOpenGLTexture> container;
    QScopedPointer<QOpenGLTexture> face;

    GLfloat mixFactor = 0.2f;

};
#endif // WIDGET_H
