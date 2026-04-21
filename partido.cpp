#include "Partido.h"
#include <iostream>
#include <cstdlib> // rand()

using namespace std;

Partido::Partido(Equipo* e1, Equipo* e2) {
    equipo1 = e1;
    equipo2 = e2;

    goles1 = 0;
    goles2 = 0;

    fecha = "20/06/2026";
    sede = "nombreSede";
}

void Partido::simular() {

    // Genero goles aleatorios
    goles1 = rand() % 5; // 0 a 4
    goles2 = rand() % 5;

    //  Actualizo estadísticas de equipos
    equipo1->actualizarEstadisticas(goles1, goles2);
    equipo2->actualizarEstadisticas(goles2, goles1);
}

void Partido::mostrarResultado() {
    cout << equipo1->getPais() << " " << goles1
         << " - " << goles2 << " "
         << equipo2->getPais() << endl;
}
