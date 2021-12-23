#include "widget.h"
#include <QImage>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QtMath>

static const float vertices[] = {
     // Vertices          // Normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , cubeBuffer(QOpenGLBuffer::VertexBuffer)
    , lampBuffer(QOpenGLBuffer::VertexBuffer)
{
}

Widget::~Widget()
{
    cubeBuffer.destroy();
    lampBuffer.destroy();
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeCubeGeometry();
    initializeLampGeometry();
    initializeShaders();
}

void Widget::resizeGL(int w, int h)
{
    camera.setViewport(w, h);
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.update();

    drawCube();
    drawLamp();

    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        camera.move(Camera::MoveDirection::Direction_Forward);
        break;
    case Qt::Key_S:
        camera.move(Camera::MoveDirection::Direction_Backward);
        break;
    case Qt::Key_A:
        camera.move(Camera::MoveDirection::Direction_Left);
        break;
    case Qt::Key_D:
        camera.move(Camera::MoveDirection::Direction_Right);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        camera.move(Camera::MoveDirection::Direction_Forward, false);
        break;
    case Qt::Key_S:
        camera.move(Camera::MoveDirection::Direction_Backward, false);
        break;
    case Qt::Key_A:
        camera.move(Camera::MoveDirection::Direction_Left, false);
        break;
    case Qt::Key_D:
        camera.move(Camera::MoveDirection::Direction_Right, false);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    static QPointF lastPos = event->pos();
    QPointF currentPos = event->pos();

    float xoffset = currentPos.x() - lastPos.x();
    float yoffset = -(currentPos.y() - lastPos.y());

    lastPos = currentPos;

    static constexpr float sensitivity = 0.2;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotate(xoffset, yoffset);
    QWidget::mouseMoveEvent(event);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    camera.zoom(event->angleDelta().y()/100.0f);
    QWidget::wheelEvent(event);
}

void Widget::initializeCubeGeometry()
{
    cubeVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&cubeVao);

    cubeBuffer.create();
    cubeBuffer.bind();
    cubeBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    cubeBuffer.allocate(vertices, sizeof(vertices));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void Widget::initializeLampGeometry()
{
    lampVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&lampVao);

    lampBuffer.create();
    lampBuffer.bind();
    lampBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    lampBuffer.allocate(vertices, sizeof(vertices));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
}

void Widget::initializeShaders()
{
    if (!cubeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcube.vsh")
            || !cubeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcube.fsh")
            || !cubeProgram.link()) {
        qCritical() << "Shader error";
        close();
    }
    if (!lampProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vlamp.vsh")
            || !lampProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/flamp.fsh")
            || !lampProgram.link()) {
        qCritical() << "Shader error";
        close();
    }
}

void Widget::drawCube()
{
    cubeProgram.bind();

    QMatrix4x4 model;
    QMatrix4x4 view = camera.view();
    QMatrix4x4 projection = camera.projection();

    cubeProgram.setUniformValue("model", model);
    cubeProgram.setUniformValue("view", view);
    cubeProgram.setUniformValue("projection", projection);

    cubeProgram.setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.31f));
    cubeProgram.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    cubeProgram.setUniformValue("lightPos", lightPos);
    cubeProgram.setUniformValue("viewPos", camera.position());


    QOpenGLVertexArrayObject::Binder vaoBinder(&cubeVao);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    cubeProgram.release();
}

void Widget::drawLamp()
{
    lampProgram.bind();

    QMatrix4x4 model;
    model.translate(lightPos);
    model.scale({0.2f, 0.2f, 0.2f});

    QMatrix4x4 view = camera.view();
    QMatrix4x4 projection = camera.projection();

    lampProgram.setUniformValue("model", model);
    lampProgram.setUniformValue("view", view);
    lampProgram.setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoBinder(&lampVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    static QElapsedTimer timer;
    if (!timer.isValid()) {
        timer.start();
    }

    lightPos.setX(qSin(qDegreesToRadians(timer.elapsed() * 0.1)));
    lightPos.setY(qSin(qDegreesToRadians(timer.elapsed() * 0.1)));

    lampProgram.release();
}

