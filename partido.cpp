#include "Partido.h"
#include <iostream>
#include <cstdlib> // rand()
#include <cmath>

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
    double mu = 1.35;
    double alpha = 0.6;
    double beta = 0.4;

    double gfA = equipo1->getPromedioGolesFavor();
    double gcB = equipo2->getPromedioGolesContra();

    double gfB = equipo2->getPromedioGolesFavor();
    double gcA = equipo1->getPromedioGolesContra();

    double lambdaA = mu * pow(gfA / mu, alpha) * pow(gcB / mu, beta);
    double lambdaB = mu * pow(gfB / mu, alpha) * pow(gcA / mu, beta);

    goles1 = rand() % ((int)lambdaA + 1);
    goles2 = rand() % ((int)lambdaB + 1);

    //  Actualizo estadísticas de equipos
    equipo1->actualizarEstadisticas(goles1, goles2);
    equipo2->actualizarEstadisticas(goles2, goles1);
}
void Partido::mostrarResultado() {
    cout << equipo1->getPais() << " " << goles1
         << " - " << goles2 << " "
         << equipo2->getPais() << endl;
}
