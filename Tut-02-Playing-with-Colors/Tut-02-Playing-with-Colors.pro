TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    FragPosition.cpp \
    VertexColors.cpp


unix: LIBS += -L$$PWD/../glsdk_0_5_2/glload/lib/ -lglloadD

INCLUDEPATH += $$PWD/../glsdk_0_5_2/glload/include
DEPENDPATH += $$PWD/../glsdk_0_5_2/glload/include

unix: PRE_TARGETDEPS += $$PWD/../glsdk_0_5_2/glload/lib/libglloadD.a

OTHER_FILES += \
    FragPosition.frag \
    FragPosition.vert \
    VertexColors.frag \
    VertexColors.vert
