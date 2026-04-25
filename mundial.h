#ifndef MUNDIAL_H
#define MUNDIAL_H
#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Grupo.h"
#include "Partido.h"

class Mundial {
private:
    Lista<Equipo*> equipos;
    Lista<Grupo*> grupos;
    Lista<Partido*> partidos;
    Lista<Equipo*> bombo1;
    Lista<Equipo*> bombo2;
    Lista<Equipo*> bombo3;
    Lista<Equipo*> bombo4;

public:
    Mundial();

    void cargarEquipos(const string& archivo);

    void crearBombos();
    Equipo* sacarValido(Lista<Equipo*>& bombo, Grupo& g);
    void formarGrupos();
    void mostrarGrupos();

    void generarPartidosGrupos();
    void simularFaseGrupos();

    void calcularClasificados();

    void generarEliminatorias();
    void simularEliminatorias();

    void mostrarEstadisticasFinales();
    string calcularFecha(string base, int dias);

    void mostrarMemoria();
    ~Mundial();
};

#endif
