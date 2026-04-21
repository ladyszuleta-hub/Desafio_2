#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
using namespace std;

#include "Equipo.h"

class Partido {
private:
    Equipo* equipo1;
    Equipo* equipo2;

    int goles1;
    int goles2;

    string fecha;
    string sede;

public:
    Partido(Equipo* e1, Equipo* e2);

    void simular();

    void mostrarResultado();
};

#endif
