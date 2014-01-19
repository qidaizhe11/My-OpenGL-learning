TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lGLEW -lglfw

SOURCES += main.cpp \
    ../common/shader.cpp

HEADERS += \
    ../common/shader.hpp

OTHER_FILES += \
    Transform.vert \
    Color.frag

