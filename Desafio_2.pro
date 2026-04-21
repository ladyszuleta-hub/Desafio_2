TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        equipo.cpp \
        jugador.cpp \
        main.cpp \
        partido.cpp

HEADERS += \
    Equipo.h \
    Lista.h \
    Nodo.h \
    jugador.h \
    partido.h
