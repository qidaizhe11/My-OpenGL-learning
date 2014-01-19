TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lGLEW -lglfw

SOURCES += main.cpp \
    ../common/shader.cpp \
    ../common/texture.cpp \
    ../common/controls.cpp

HEADERS += \
    ../common/shader.hpp \
    ../common/texture.h \
    ../common/controls.h

