#include "widget.h"
#include <QImage>

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
    container = initializeTextures(":/container.jpg");
    face = initializeTextures(":/awesomeface.png");
}

void Widget::resizeGL(int w, int h)
{
    Q_UNUSED(w) Q_UNUSED(h)
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    program.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, face);

    program.setUniformValue("ourTexture1", 0);
    program.setUniformValue("ourTexture2", 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    program.release();
}

void Widget::initializeTriangle()
{
    static const GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    static const GLuint indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    vertexArrayObject.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer.allocate(vertices, sizeof(vertices));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indexBuffer.allocate(indices, sizeof(indices));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    // attribute with color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // attribute with texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6* sizeof(GLfloat)));
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

GLuint Widget::initializeTextures(const QString &path)
{
    GLuint texture = 0;
    QImage image(path);
    if (image.isNull()) {
        qCritical() << "Texture error";
        return 0;
    }

    image = image.convertToFormat(QImage::Format_RGB888);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

