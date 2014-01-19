TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lGLEW -lglfw

SOURCES += main.cpp \
    ../common/texture.cpp \
    ../common/shader.cpp

HEADERS += \
    ../common/texture.h \
    ../common/shader.hpp

OTHER_FILES += \
    Texture.frag \
    Transform.vert

