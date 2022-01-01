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

struct Light {
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    bool operator==(const Light &other)
    {
        return other.ambient == ambient && other.specular == specular && other.diffuse == diffuse;
    }
    bool operator!=(const Light &other)
    {
        return !(*this == other);
    }
};
Q_DECLARE_METATYPE(Light)

class Widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    Q_PROPERTY(Light directionalLight READ directionalLight WRITE setDirectionalLight NOTIFY directionalLightChanged)
    Q_PROPERTY(Light pointLight READ pointLight WRITE setPointLight NOTIFY pointLightChanged)
    Q_PROPERTY(Light spotLight READ spotLight WRITE setSpotLight NOTIFY spotLightChanged)

public:
    Widget(QColor background = QColor::fromRgbF(0.2f, 0.3f, 0.3f, 1.0f), QWidget *parent = nullptr);
    ~Widget();

    Light directionalLight() const;
    Light pointLight() const;
    Light spotLight() const;

    void setDirectionalLight(const Light &light);
    void setPointLight(const Light &light);
    void setSpotLight(const Light &light);

    Q_SIGNAL void directionalLightChanged(const Light &newLight);
    Q_SIGNAL void pointLightChanged(const Light &newLight);
    Q_SIGNAL void spotLightChanged(const Light &newLight);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void initializeCubeGeometry();
    void initializeLampGeometry();
    void initializeShaders();
    QOpenGLTexture *initializeTexture(const QString &path);


    void drawCube(const QVector3D &position, float angle);
    void drawLamp(const QVector3D &position);

    QOpenGLBuffer cubeBuffer;
    QOpenGLBuffer lampBuffer;
    QOpenGLVertexArrayObject cubeVao;
    QOpenGLVertexArrayObject lampVao;
    QOpenGLShaderProgram cubeProgram;
    QOpenGLShaderProgram lampProgram;
    QOpenGLTexture *container = nullptr;
    QOpenGLTexture *container_specular = nullptr;

    const QList<QVector3D> lightPositions = {
        { 0.7f,  0.2f,  2.0f},
        { 2.3f, -3.3f, -4.0f},
        {-4.0f,  2.0f, -12.0f},
        { 0.0f,  0.0f, -3.0f}
    };

    Camera camera;

    const QColor m_background;

    Light m_directionalLight = {
        QVector3D(0.2f, 0.2f, 0.2f),
        QVector3D(0.5f, 0.5f, 0.5f),
        QVector3D(1.0f, 1.0f, 1.0f)
    };

    Light m_pointLight = {
        QVector3D(0.2f, 0.2f, 0.2f),
        QVector3D(0.5f, 0.5f, 0.5f),
        QVector3D(1.0f, 1.0f, 1.0f)
    };

    Light m_spotLight = {
        QVector3D(0.2f, 0.2f, 0.2f),
        QVector3D(0.5f, 0.5f, 0.5f),
        QVector3D(1.0f, 1.0f, 1.0f)
    };
};
#endif // WIDGET_H
