TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../common/shader.cpp

LIBS += -lglfw -lGL -lGLU -lGLEW -lglut

#INCLUDEPATH += "/home/daizhe/Codes/OpenGL-learning/common"

OTHER_FILES += \
    SingleColor.frag \
    SimpleTransform.vert

HEADERS += \
    ../common/shader.hpp

