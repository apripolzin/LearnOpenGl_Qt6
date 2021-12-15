#include "widget.h"
#include <QImage>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QDateTime>
#include <QKeyEvent>
#include <QtMath>

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
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeTriangle();
    initializeShaders();
    container.reset(initializeTextures(":/container.jpg"));
    face.reset(initializeTextures(":/awesomeface.png"));
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();

    glActiveTexture(GL_TEXTURE0);
    container->bind();

    glActiveTexture(GL_TEXTURE1);
    face->bind();

    QMatrix4x4 view, projection;

    doMovement();
    doRotation();

    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projection.perspective(fov, 1.0f * width() / height(), 0.1f, 100.0f);

    program.setUniformValue("ourTexture1", 0);
    program.setUniformValue("ourTexture2", 1);

    program.setUniformValue("view", view);
    program.setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    int i = 0;
    for (const auto &position : positions()) {
        QMatrix4x4 model;
        model.translate(position);

        const QTime time = QTime::currentTime();

        const float delta_angle = (time.second() * 1000 + time.msec()) / 25 % 360;
        const float angle = 20.0f * i + delta_angle;
        model.rotate(angle, {0.5, 1.0, 0.0});
        ++i;

        program.setUniformValue("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    program.release();
    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    keys[event->key()] = true;
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    keys[event->key()] = false;
    QWidget::keyPressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    updateAngles(event->position());
    QWidget::mouseMoveEvent(event);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    updateFov(event->angleDelta().y()/100.0f);
    QWidget::wheelEvent(event);

}

void Widget::initializeTriangle()
{
    static const GLfloat vertices[] = {
         // Positions    // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer.allocate(vertices, sizeof(vertices));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    // attribute with texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
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

void Widget::doMovement()
{
    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    static qint64 lastTime = 0;

    const qint64 deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    const float cameraSpeed = 0.5f / deltaTime;
    if (keys.value(Qt::Key_W) ) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keys.value(Qt::Key_S)) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keys.value(Qt::Key_A)) {
        cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized()  * cameraSpeed;
    }
    if (keys.value(Qt::Key_D)) {
        cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized()  * cameraSpeed;
    }
}

void Widget::doRotation()
{
    QVector3D front;
    front.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    front.setY(qSin(qDegreesToRadians(pitch)));
    front.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));

    cameraFront = front.normalized();
}

void Widget::updateAngles(const QPointF &mousePos)
{
    static QPointF lastPos = mousePos;
    QPointF currentPos = mousePos;

    float xoffset = currentPos.x() - lastPos.x();
    float yoffset = -(currentPos.y() - lastPos.y());

    lastPos = currentPos;

    const float sensitivity = 0.2;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    pitch = qBound(-89.0f, pitch, 89.0f);
}

void Widget::updateFov(float angle)
{
    fov -= angle;
    fov = qBound(1.0f, fov, 45.0f);
}

const QVector<QVector3D> &Widget::positions()
{
    static const QVector<QVector3D> positions = {
        QVector3D( 0.0f,  0.0f,  0.0f),
        QVector3D( 2.0f,  5.0f, -15.0f),
        QVector3D(-1.5f, -2.2f, -2.5f),
        QVector3D(-3.8f, -2.0f, -12.3f),
        QVector3D( 2.4f, -0.4f, -3.5f),
        QVector3D(-1.7f,  3.0f, -7.5f),
        QVector3D( 1.3f, -2.0f, -2.5f),
        QVector3D( 1.5f,  2.0f, -2.5f),
        QVector3D( 1.5f,  0.2f, -1.5f),
        QVector3D(-1.3f,  1.0f, -1.5f)
    };
    return positions;
}

QOpenGLTexture *Widget::initializeTextures(const QString &path)
{
    QOpenGLTexture *texture = new QOpenGLTexture(QImage(path).mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}

