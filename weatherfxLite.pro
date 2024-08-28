TEMPLATE = app
TARGET = weatherfxLite
INCLUDEPATH += . include 

# Uncomment the following line to use the Celsius scale
# DEFINES += CELSIUS

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
