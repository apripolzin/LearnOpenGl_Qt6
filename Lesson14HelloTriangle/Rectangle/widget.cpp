#include "widget.h"
#include <QList>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , vertexBuffer(QOpenGLBuffer::VertexBuffer)
    , indexBuffer(QOpenGLBuffer::IndexBuffer)
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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    program.release();
}

void Widget::initializeTriangle()
{
    static const GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    static const GLuint indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer.allocate(vertices, sizeof(vertices));

    indexBuffer.create();
    indexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indexBuffer.allocate(indices, sizeof(indices));

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

