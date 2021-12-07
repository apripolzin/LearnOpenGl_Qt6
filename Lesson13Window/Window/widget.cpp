#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Widget::resizeGL(int w, int h)
{
    Q_UNUSED(w) Q_UNUSED(h)
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

