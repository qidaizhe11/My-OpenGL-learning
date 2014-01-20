TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lGLEW -lglfw

SOURCES += main.cpp \
    ../common/controls.cpp \
    ../common/shader.cpp \
    ../common/texture.cpp \
    ../common/objloader.cpp

OTHER_FILES += \
    Texture.frag \
    Transform.vert

HEADERS += \
    ../common/controls.h \
    ../common/shader.hpp \
    ../common/texture.h \
    ../common/objloader.h

