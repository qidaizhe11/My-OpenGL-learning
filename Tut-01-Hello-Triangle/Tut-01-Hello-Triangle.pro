TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

#INCLUDEPATH += "../glsdk_0_5_2"

unix:!macx: LIBS += -L$$PWD/../glsdk_0_5_2/glload/lib/ -lglloadD

INCLUDEPATH += $$PWD/../glsdk_0_5_2/glload/include
DEPENDPATH += $$PWD/../glsdk_0_5_2/glload/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../glsdk_0_5_2/glload/lib/libglloadD.a
