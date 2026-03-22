TEMPLATE = app
TARGET = weatherfxLite
macx: QMAKE_MACOSX_DEPLOYMENT_TARGET = $$system(sw_vers -productVersion)
INCLUDEPATH += . include 
INCLUDEPATH += /opt/homebrew/include

#INCLUDEPATH += /opt/homebrew/opt/openssl@3/include
LIBS += -L/opt/homebrew/lib

LIBS += -lzmq

QT += widgets network

# Input
FORMS   += weatherFxLite.ui
HEADERS += WeatherKitAPI.h FileDownloader.h WeatherFXLite.h WindArrow.h config.h ZmqListener.h
SOURCES += WeatherKitAPI.cpp main.cpp FileDownloader.cpp WeatherFXLite.cpp WindArrow.cpp ZmqListener.cpp

# Output
DESTDIR = build
OBJECTS_DIR = $$DESTDIR/.obj
