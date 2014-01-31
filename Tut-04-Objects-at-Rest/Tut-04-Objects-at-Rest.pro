TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lfreeglut

unix: LIBS += -L$$PWD/../glsdk/glload/lib/ -lglloadD

INCLUDEPATH += $$PWD/../glsdk/glload/include
DEPENDPATH += $$PWD/../glsdk/glload/include

unix: PRE_TARGETDEPS += $$PWD/../glsdk/glload/lib/libglloadD.a

SOURCES += main.cpp \
    OrthoCube.cpp

OTHER_FILES += \
    data/OrthoWithOffset.vert \
    data/StandardColors.frag

