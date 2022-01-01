#include "widget.h"
#include <QSurfaceFormat>
#include <QApplication>
#include <QWidget>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Widget *desert  = new Widget(QColor::fromRgbF(0.75f, 0.52f, 0.3f, 1.0f));
    Widget *factory = new Widget(QColor::fromRgbF(0.1f, 0.1f, 0.1f, 1.0f));
    Widget *horror  = new Widget(QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f));
    Widget *lab     = new Widget(QColor::fromRgbF(0.9f, 0.9f, 0.9f, 1.0f));
    {
        Light dirLight;
        dirLight.ambient = {0.3f, 0.24f, 0.14f};
        dirLight.diffuse = {0.7f, 0.42f, 0.26f};
        dirLight.specular = {0.5f, 0.5f, 0.5f};

        Light pointLight;
        pointLight.ambient = {1.0f, 0.6f, 0.0f};
        pointLight.diffuse = {1.0f, 0.6f, 0.0f};
        pointLight.specular = {1.0f, 0.6f, 0.0f};

        Light spotLight;
        spotLight.ambient = {0.0f, 0.0f, 0.0f};
        spotLight.diffuse = {0.8f, 0.8f, 0.0f};
        spotLight.specular = {0.8f, 0.8f, 0.0f};

        desert->setDirectionalLight(dirLight);
        desert->setPointLight(pointLight);
        desert->setSpotLight(spotLight);
    }
    {
        Light dirLight;
        dirLight.ambient = {0.05f, 0.05f, 0.1f};
        dirLight.diffuse = {0.2f, 0.2f, 0.7};
        dirLight.specular = {0.7f, 0.7f, 0.7f};

        Light pointLight;
        pointLight.ambient = {0.2f, 0.2f, 0.6f};
        pointLight.diffuse = {0.2f, 0.2f, 0.6f};
        pointLight.specular = {0.2f, 0.2f, 0.6f};

        Light spotLight;
        spotLight.ambient = {0.0f, 0.0f, 0.0f};
        spotLight.diffuse = {1.0f, 1.0f, 1.0f};
        spotLight.specular = { 1.0f, 1.0f, 1.0f};

        factory->setDirectionalLight(dirLight);
        factory->setPointLight(pointLight);
        factory->setSpotLight(spotLight);
    }
    {
        Light dirLight;
        dirLight.ambient = {0.0f, 0.0f, 0.0f};
        dirLight.diffuse = {0.05f, 0.05f, 0.05};
        dirLight.specular = {0.2f, 0.2f, 0.2f};

        Light pointLight;
        pointLight.ambient = {0.1f, 0.1f, 0.1f};
        pointLight.diffuse = {0.1f, 0.1f, 0.1f};
        pointLight.specular = {0.1f, 0.1f, 0.1f};

        Light spotLight;
        spotLight.ambient = {0.0f, 0.0f, 0.0f};
        spotLight.diffuse = {1.0f, 1.0f, 1.0f};
        spotLight.specular = { 1.0f, 1.0f, 1.0f};

        horror->setDirectionalLight(dirLight);
        horror->setPointLight(pointLight);
        horror->setSpotLight(spotLight);
    }
    {
        Light dirLight;
        dirLight.ambient = {0.5f, 0.5f, 0.5f};
        dirLight.diffuse = {1.0f, 1.0f, 1.0f};
        dirLight.specular = {1.0f, 1.0f, 1.0f};

        Light pointLight;
        pointLight.ambient = {0.4f, 0.7f, 0.1f};
        pointLight.diffuse = {0.4f, 0.7f, 0.1f};
        pointLight.specular = {0.4f, 0.7f, 0.1f};

        Light spotLight;
        spotLight.ambient = {0.0f, 0.0f, 0.0f};
        spotLight.diffuse = {0.0f, 1.0f, 0.0f};
        spotLight.specular = {0.0f, 1.0f, 0.0f};

        lab->setDirectionalLight(dirLight);
        lab->setPointLight(pointLight);
        lab->setSpotLight(spotLight);
    }

    QWidget container;
    container.resize(800, 600);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(desert, 0, 0);
    layout->addWidget(factory, 0, 1);
    layout->addWidget(horror, 1, 0);
    layout->addWidget(lab, 1, 1);
    container.setLayout(layout);
    container.show();


    return a.exec();
}
