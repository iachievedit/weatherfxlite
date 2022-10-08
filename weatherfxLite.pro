TEMPLATE = app
TARGET = weatherfxLite
INCLUDEPATH += . include 

#INCLUDEPATH += /opt/homebrew/opt/openssl@3/include
#LIBS += -L/opt/homebrew/opt/openssl@3/lib -lcrypto

QT += widgets network

# Input
FORMS   += weatherFxLite.ui
HEADERS += WeatherKitAPI.h FileDownloader.h WeatherFXLite.h config.h
SOURCES += WeatherKitAPI.cpp main.cpp FileDownloader.cpp WeatherFXLite.cpp

# Output
DESTDIR = build
OBJECTS_DIR = $$DESTDIR/.obj
