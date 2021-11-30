QT -= gui
QT += opengl

TEMPLATE = lib
CONFIG += staticlib c++17
DESTDIR = ../bin

SOURCES += \
    src/Camera.cpp \
    src/Defaults.cpp \
    src/Generator.cpp \
    src/Geometry.cpp \
    src/Item.cpp \
    src/Manipulator.cpp \
    src/Mesh.cpp \
    src/Pipe.cpp \
    src/Program.cpp \
    src/Projection.cpp \
    src/Scene.cpp \
    src/ScenePipe.cpp \
    src/Utils.cpp \
    src/View.cpp

HEADERS += \
    inc/Camera.h \
    inc/Common.h \
    inc/Defaults.h \
    inc/Figures.h \
    inc/Generator.h \
    inc/Geometry.h \
    inc/Item.h \
    inc/Light.h \
    inc/Manipulator.h \
    inc/Material.h \
    inc/Mesh.h \
    inc/Pipe.h \
    inc/Program.h \
    inc/Projection.h \
    inc/Scene.h \
    inc/ScenePipe.h \
    inc/Utils.h \
    inc/View.h

INCLUDEPATH += inc

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
