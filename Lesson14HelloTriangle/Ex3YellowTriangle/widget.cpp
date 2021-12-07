#include "widget.h"
#include <QList>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , vbo_firstTriangle(QOpenGLBuffer::VertexBuffer)
    , vbo_secondTriangle(QOpenGLBuffer::VertexBuffer)
{
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeFirstTriangle();
    initializeSecondTriangle();

    initializeRedShaders();
    initializeYellowShaders();
}

void Widget::resizeGL(int w, int h)
{
    Q_UNUSED(w) Q_UNUSED(h)
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    {
        redRrogram.bind();
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao_firstTriamgle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        redRrogram.release();
    }
    {
        yellowProgram.bind();
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao_secondTriangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        yellowProgram.release();
    }
}

void Widget::initializeFirstTriangle()
{
    static const GLfloat vertices[] = {
        -0.5f, 0.5f, 0.0f, // first triangle
         0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
    };

    vao_firstTriamgle.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao_firstTriamgle);

    vbo_firstTriangle.create();
    vbo_firstTriangle.bind();
    vbo_firstTriangle.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo_firstTriangle.allocate(vertices, sizeof(vertices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
}

void Widget::initializeSecondTriangle()
{
    static const GLfloat vertices[] = {
        -0.5f, 0.5f, 0.0f, // second triangle
         0.5f, -0.5f, 0.0f,
         -0.5f, -0.5f, 0.0f,
    };

    vao_secondTriangle.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao_secondTriangle);

    vbo_secondTriangle.create();
    vbo_secondTriangle.bind();
    vbo_secondTriangle.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo_secondTriangle.allocate(vertices, sizeof(vertices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

}

void Widget::initializeRedShaders()
{
    if (!redRrogram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh")
            || !redRrogram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh")
            || !redRrogram.link()) {
        qCritical() << "Shader error";
        close();
    }
}

void Widget::initializeYellowShaders()
{
    if (!yellowProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh")
            || !yellowProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/yellowshader.fsh")
            || !yellowProgram.link()) {
        qCritical() << "Shader error";
        close();
    }
}

