TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += *.cpp \
    mainloop.cpp

HEADERS += *.h \ 
    mainloop.h

LIBS += -lSDL2

