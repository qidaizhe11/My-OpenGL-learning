TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLU -lfreeglut

unix: LIBS += -L$$PWD/../glsdk/glload/lib/ -lglloadD

INCLUDEPATH += $$PWD/../glsdk/glload/include
DEPENDPATH += $$PWD/../glsdk/glload/include

unix: PRE_TARGETDEPS += $$PWD/../glsdk/glload/lib/libglloadD.a

unix: LIBS += -L$$PWD/../glsdk/glutil/lib/ -lglutilD

INCLUDEPATH += $$PWD/../glsdk/glutil/include
DEPENDPATH += $$PWD/../glsdk/glutil/include

unix: PRE_TARGETDEPS += $$PWD/../glsdk/glutil/lib/libglutilD.a

SOURCES += main.cpp \
    WorldScene.cpp

OTHER_FILES += \
    data/PosOnlyWorldTransform.vert \
    data/ColorUniform.frag \
    data/PosColorWorldTransform.vert \
    data/ColorMultUniform.frag
