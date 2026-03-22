QT += widgets

TARGET   = conditiontest
macx: QMAKE_MACOSX_DEPLOYMENT_TARGET = $$system(sw_vers -productVersion)
TEMPLATE = app

SOURCES  += conditiontest.cpp
FORMS    += weatherfxLite.ui

HEADERS  += WindArrow.h
SOURCES  += WindArrow.cpp

INCLUDEPATH += include
