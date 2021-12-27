QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    camera.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    camera.h \
    widget.h

RESOURCES += \
    shaders.qrc \
    textures.qrc
