#ifndef MUNDIAL_H
#define MUNDIAL_H

#include "Lista.h"
#include "Equipo.h"
#include "Grupo.h"
#include "Partido.h"

class Mundial {
private:
    Lista<Equipo*> equipos;
    Lista<Grupo> grupos;
    Lista<Partido*> partidos;

public:
    Mundial();

    void cargarEquipos(const string& archivo);

    void crearBombos();
    void formarGrupos();
    void mostrarGrupos();

    void generarPartidosGrupos();
    void simularFaseGrupos();

    void calcularClasificados();

    void generarEliminatorias();
    void simularEliminatorias();

    void mostrarEstadisticasFinales();

    void mostrarMemoria();
};

#endif
