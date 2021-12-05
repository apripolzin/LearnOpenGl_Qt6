#include "widget.h"
#include <QList>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
}

Widget::~Widget()
{
    vertexArrayObject.destroy();
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeTriangle();
    initializeShaders();
}

void Widget::resizeGL(int w, int h)
{

}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    program.bind();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    program.release();
}

void Widget::initializeTriangle()
{
    static const GLfloat vertices[] = {
        -0.5f, 0.5f, 0.0f, // first triangle
         0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,

        -0.5f,  0.5f, 0.0f, // second triangle
         0.5f, -0.5f, 0.0f,
         -0.5, -0.5f, 0.0f
    };

    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer.allocate(vertices, sizeof(vertices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

void Widget::initializeShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh")
            || !program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh")
            || !program.link()) {
        qCritical() << "Shader error";
        close();
    }
}

