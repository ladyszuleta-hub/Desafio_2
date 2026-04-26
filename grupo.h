#ifndef GRUPO_H
#define GRUPO_H

#include "Equipo.h"
#include "Lista.h"

class Grupo {
private:
    char letra;
    Lista<Equipo*> equipos;

public:
    // Constructor
    Grupo(char letra);
    Grupo(); //Constructor vacio

    // Métodos
    void quitarUltimo();
    bool puedeAgregar(Equipo* e);
    void agregarEquipo(Equipo* e);
    void mostrarEquipos() const;
    Equipo* getEquipo(int i);
    char getLetra() const;
    int getTamano() const;
};

#endif
