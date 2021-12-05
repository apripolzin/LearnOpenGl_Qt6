#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

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

private:
    void initializeFirstTriangle();
    void initializeSecondTriangle();
    void initializeShaders();

    QOpenGLVertexArrayObject vao_firstTriamgle;
    QOpenGLBuffer vbo_firstTriangle;

    QOpenGLVertexArrayObject vao_secondTriangle;
    QOpenGLBuffer vbo_secondTriangle;

    QOpenGLShaderProgram program;
};
#endif // WIDGET_H
