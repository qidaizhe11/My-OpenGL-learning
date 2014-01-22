TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lGLEW -lglfw

SOURCES += main.cpp \
    ../common/controls.cpp \
    ../common/objloader.cpp \
    ../common/shader.cpp \
    ../common/texture.cpp

OTHER_FILES += \
    StandardShading.frag \
    StandardShading.vert

HEADERS += \
    ../common/controls.h \
    ../common/objloader.h \
    ../common/shader.hpp \
    ../common/texture.h

