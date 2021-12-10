#include "widget.h"
#include <QSurfaceFormat>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Widget w;
    w.show();
    return a.exec();
}
