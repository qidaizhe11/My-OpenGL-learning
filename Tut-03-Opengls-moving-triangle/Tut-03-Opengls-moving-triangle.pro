TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    cpuPositionOffset.cpp \
    vertPositionOffset.cpp \
    vertCalcOffset.cpp


unix: LIBS += -L$$PWD/../glsdk/glload/lib/ -lglloadD

INCLUDEPATH += $$PWD/../glsdk/glload/include
DEPENDPATH += $$PWD/../glsdk/glload/include

unix: PRE_TARGETDEPS += $$PWD/../glsdk/glload/lib/libglloadD.a

OTHER_FILES += \
    data/standard.frag \
    data/standard.vert \
    data/positionOffset.vert \
    data/calcOffset.vert
