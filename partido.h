#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
using namespace std;
#include "Equipo.h"
class Enfrentamiento {
public:
    Equipo* a;
    Equipo* b;

    Enfrentamiento() {
        a = nullptr;
        b = nullptr;
    }

    Enfrentamiento(Equipo* a, Equipo* b) {
        this->a = a;
        this->b = b;
    }
};
class StatsJugadorPartido {
public:
    Jugador* jugador;
    short int goles;
    short int amarillas;
    short int rojas;
    short int faltas;
    short int minutos;
};
enum Fase {
    GRUPOS,
    R16,
    R8,
    QF,
    SF,
    FINAL
};
class Partido {
private:

    Equipo* equipo1;
    Equipo* equipo2;

    short int goles1;
    short int goles2;

    string fecha;
    string hora;
    string sede;

    string arbitro1;
    string arbitro2;
    string arbitro3;


    Fase fase;

    StatsJugadorPartido* jugadoresA;
    StatsJugadorPartido* jugadoresB;

    string getFecha() const;

    string faseActual;
    void calcularLambdas(double& lambdaA, double& lambdaB);

    short int generarGoles(double lambda);

    short int simularTiempo(int& amarillasA, int& rojasA,int& amarillasB, int& rojasB);

    void seleccionarJugadores();
    void repartirGoles();
    void actualizarJugadores();
    void actualizarEquipos(int amarillasA, int rojasA,int amarillasB, int rojasB);

public:
    Partido(Equipo* e1, Equipo* e2, string fecha, string hora,
            string sede, string a1, string a2, string a3, Fase f);
    ~Partido();

    void simular();
    void mostrarResultado();
    Equipo* getEquipo1() const { return equipo1; }
    Equipo* getEquipo2() const { return equipo2; }
    bool empate() const;
    void desempatarPorRanking();
    Equipo* getGanador();

    short int getGoles1() const { return goles1; }
    short int getGoles2() const { return goles2; }
};

#endif
