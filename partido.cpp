#include "Partido.h"
#include <iostream>
#include <cstdlib> // rand()
#include <cmath>
#include <string>

using namespace std;

Partido::Partido(Equipo* e1, Equipo* e2, string fecha, string hora,
        string sede, string a1, string a2, string a3, Fase f) {

    equipo1 = e1;
    equipo2 = e2;

    this->fecha = fecha;
    this->hora = hora;
    this->sede = sede;

    arbitro1 = a1;
    arbitro2 = a2;
    arbitro3 = a3;

    fase = f;

    goles1 = 0;
    goles2 = 0;
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
