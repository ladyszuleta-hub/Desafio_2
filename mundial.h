#ifndef MUNDIAL_H
#define MUNDIAL_H
#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "grupo.h"
#include "partido.h"
#include "tablagrupo.h"

    using namespace std;

class Mundial {

private:
    Equipo* campeon;

    Lista<Equipo*> equipos;
    Lista<Grupo*> grupos;

    Lista<Equipo*> bombo1;
    Lista<Equipo*> bombo2;
    Lista<Equipo*> bombo3;
    Lista<Equipo*> bombo4;
    Lista<Partido*> partidos;
    Equipo** equiposControl;   // punteros a equipos
    int* ultimoDia;            // último día que jugaron
    int totalEquiposControl;


    Lista<Partido*> partidosR16;

    Lista<Equipo*> primeros;
    Lista<Equipo*> segundos;
    Lista<StatsEquipo*> terceros;

    Lista<Equipo*> clasificadosR16;

    Lista<Equipo*> clasificadosR8;
    Lista<Equipo*> clasificadosR4;

    Lista<Equipo*> todosLosJugadores; // para estadísticas
    Lista<Equipo*> equiposR16;
    Lista<Equipo*> equiposR8;
    Lista<Equipo*> equiposR4;


    Lista<TablaGrupo*> tablas;


    Lista<Enfrentamiento> historial;


    long long iteracionesGlobal = 0;
    long long memoriaGlobal = 0;

public:


    Mundial();
    ~Mundial();


    void cargarEquipos(const string& archivo);
    void mezclarLista(Lista<Equipo*>& lista);
    void crearBombos();
    void formarGrupos();
    void mostrarGrupos();
    void registrarPartido(Equipo* e1, Equipo* e2, int dia);
    bool puedeJugar(Equipo* e, int diaActual);
    int buscarEquipoIndex(Equipo* e);
    void generarPartidosGrupos();
    void simularFaseGrupos();
    void crearTablas();
    void mostrarTablas();


    void calcularClasificados();
    Lista<Equipo*> obtenerClasificados();

    void ordenarTerceros(Lista<StatsEquipo*>& terceros);

    // R16
    void generarR16();
    void simularR16();
    void mostrarR16();


    Lista<Partido*> generarFase(Lista<Equipo*>& equipos, Fase f);
    void simularFase(Lista<Partido*>& fase, Lista<Equipo*>& clasificados);

    Partido* crearCruceValido(Lista<Equipo*>& A, Lista<Equipo*>& B);

    void registrarEnfrentamiento(Equipo* a, Equipo* b);
    bool yaSeEnfrentaron(Equipo* a, Equipo* b);


    void mostrarTop4();
    void maximoGoleadorCampeon(Equipo* campeon);
    void top3Goleadores();
    void equipoMasGoleador();
    void mostrarEstadisticasFinales();
    void iniciar();
    bool asignar(int grupoIdx, int bomboIdx);


    void mostrarInformeFinal(Equipo* campeon);

    string calcularFecha(string base, int dias);

    void mostrarMemoria();
    void liberarPartidos(Lista<Partido*>& lista);
    void guardarJugadoresCSV();
    void mostrarConfederacionDominante(Lista<Equipo*>& lista, string fase);


};

#endif
