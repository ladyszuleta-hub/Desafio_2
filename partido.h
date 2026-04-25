#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
using namespace std;

#include "Equipo.h"
enum Fase {
    GRUPOS,
    R16,
    R8,
    QF,
    SF,
    TERCER_PUESTO,
    FINAL
};
class Partido {
private:
    Equipo* equipo1;
    Equipo* equipo2;

    int goles1;
    int goles2;

    string fecha;
    string hora;
    string sede;

    string arbitro1;
    string arbitro2;
    string arbitro3;


    Fase fase;

public:
    Partido(Equipo* e1, Equipo* e2, string fecha, string hora,
            string sede, string a1, string a2, string a3, Fase f);

    void simular();
    void mostrarResultado();
    Equipo* getEquipo1() const { return equipo1; }
    Equipo* getEquipo2() const { return equipo2; }
    bool empate() const;
    void desempatarPorRanking();
    Equipo* getGanador();

    int getGoles1() const { return goles1; }
    int getGoles2() const { return goles2; }
};

#endif
