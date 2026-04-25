#ifndef MUNDIAL_H
#define MUNDIAL_H
#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Grupo.h"
#include "Partido.h"
#include "TablaGrupo.h"

class Mundial {
private:

    Lista<Equipo*> equipos;
    Lista<Grupo*> grupos;
    Lista<Partido*> partidos;
    Lista<Partido*> partidosR16;
    Lista<Equipo*> bombo1;
    Lista<Equipo*> bombo2;
    Lista<Equipo*> bombo3;
    Lista<Equipo*> bombo4;
    Lista<TablaGrupo*> tablas;
    Lista<Equipo*> clasificadosR16;
    Lista<Equipo*> primeros;
    Lista<Equipo*> segundos;
    Lista<Equipo*> terceros;

public:
    Mundial();
    ~Mundial();

    void cargarEquipos(const string& archivo);

    void crearBombos();

    void formarGrupos();
    bool asignar(int grupoIdx, int bomboIdx);
    void mostrarGrupos();

    void generarPartidosGrupos();
    void simularFaseGrupos();

    void calcularClasificados();

    void generarEliminatorias();
    void simularEliminatorias();

    void mostrarEstadisticasFinales();
    string calcularFecha(string base, int dias);
    void mezclarLista(Lista<Equipo*>& lista);
    void iniciar();

    void crearTablas();
    void mostrarTablas();

    void ordenarTerceros(Lista<StatsEquipo*>& terceros);

    // R16
    void generarR16();
    void simularR16();
    Partido* crearCruceValido(Lista<Equipo*>& A, Lista<Equipo*>& B);
    void mostrarR16();

    Lista<Equipo*> obtenerClasificados();

    void mostrarMemoria();

};

#endif
