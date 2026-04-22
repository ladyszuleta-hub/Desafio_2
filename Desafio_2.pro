TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        equipo.cpp \
        grupo.cpp \
        jugador.cpp \
        main.cpp \
        mundial.cpp \
        partido.cpp

HEADERS += \
    Equipo.h \
    Lista.h \
    Nodo.h \
    grupo.h \
    jugador.h \
    mundial.h \
    partido.h
