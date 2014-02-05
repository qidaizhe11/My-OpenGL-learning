TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lfreeglut

unix: LIBS += -L$$PWD/../glsdk/glload/lib/ -lglloadD

INCLUDEPATH += $$PWD/../glsdk/glload/include
DEPENDPATH += $$PWD/../glsdk/glload/include

unix: PRE_TARGETDEPS += $$PWD/../glsdk/glload/lib/libglloadD.a

SOURCES += main.cpp \
    Translation.cpp \
    Scale.cpp \
    Rotation.cpp

OTHER_FILES += \
    data/PosColorLocalTransform.vert \
    data/ColorPassthrough.frag

