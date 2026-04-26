#include "Partido.h"
#include "utilidades.h"
#include <iostream>
#include <cstdlib> // rand()
#include <cmath>
#include <ctime>
#include <string>
using namespace std;

short int generarPoisson(double lambda) {
    double L = exp(-lambda);
    int k = 0;
    double p = 1.0;

    do {
        k++;
        p *= (double)rand() / RAND_MAX;
        ITERACIONES++;
    } while (p > L);

    return k - 1;
}
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

    jugadoresA = new StatsJugadorPartido[11];
    jugadoresB = new StatsJugadorPartido[11];
}

void Partido::calcularLambdas(double& lambdaA, double& lambdaB) {

    double mu = 1.35, alpha = 0.6, beta = 0.4;

    double gfA = equipo1->getPromedioGolesFavor();
    double gcA = equipo1->getPromedioGolesContra();

    double gfB = equipo2->getPromedioGolesFavor();
    double gcB = equipo2->getPromedioGolesContra();

    lambdaA = mu * pow(gfA / mu, alpha) * pow(gcB / mu, beta);
    lambdaB = mu * pow(gfB / mu, alpha) * pow(gcA / mu, beta);

    lambdaA = max(0.0, min(lambdaA, 5.0));
    lambdaB = max(0.0, min(lambdaB, 5.0));
}
short int Partido::generarGoles(double lambda) {

    int goles = 0;

    for (int i = 0; i < 10; i++) {
        ITERACIONES++;
        if ((rand() / (double)RAND_MAX) < lambda / 10.0)
            goles++;
    }

    return goles;
}
short int Partido::simularTiempo(int& amarillasA, int& rojasA,int& amarillasB, int& rojasB) {

    for (int minuto = 1; minuto <= 90; minuto++) {
        ITERACIONES++;

        if (rand() % 1000 < 3) amarillasA++;
        if (rand() % 1000 < 3) amarillasB++;

        if (rand() % 2000 < 2) rojasA++;
        if (rand() % 2000 < 2) rojasB++;
    }

    if (goles1 == goles2) {

        for (int minuto = 91; minuto <= 120; minuto++) {
            ITERACIONES++;

            if (rand() % 1000 < 4) goles1++;
            if (rand() % 1000 < 4) goles2++;

            if (rand() % 1000 < 2) amarillasA++;
            if (rand() % 1000 < 2) amarillasB++;
        }

        return 120;
    }

    return 90;
}
void Partido::seleccionarJugadores() {

    for (int i = 0; i < 11; i++) {
        ITERACIONES++;

        int idA = rand() % equipo1->getCantidadJugadores();
        int idB = rand() % equipo2->getCantidadJugadores();

        jugadoresA[i].jugador = equipo1->getJugador(idA);
        jugadoresB[i].jugador = equipo2->getJugador(idB);

        jugadoresA[i] = {jugadoresA[i].jugador,0,0,0,0,90};
        jugadoresB[i] = {jugadoresB[i].jugador,0,0,0,0,90};
    }
}
void Partido::repartirGoles() {

    for (int i = 0; i < goles1; i++) {
        ITERACIONES++;
        jugadoresA[rand() % 11].goles++;
    }

    for (int i = 0; i < goles2; i++) {
        ITERACIONES++;
        jugadoresB[rand() % 11].goles++;
    }
}
void Partido::actualizarJugadores() {

    for (int i = 0; i < 11; i++) {
        ITERACIONES++;

        jugadoresA[i].jugador->jugarPartido(90);
        jugadoresB[i].jugador->jugarPartido(90);

        for (int g = 0; g < jugadoresA[i].goles; g++) {
            ITERACIONES++;
            jugadoresA[i].jugador->anotarGol();
        }

        for (int g = 0; g < jugadoresB[i].goles; g++) {
            ITERACIONES++;
            jugadoresB[i].jugador->anotarGol();
        }
    }
}
void Partido::actualizarEquipos(int amarillasA, int rojasA,int amarillasB, int rojasB) {

    equipo1->actualizarEstadisticas(goles1, goles2);
    equipo2->actualizarEstadisticas(goles2, goles1);

    equipo1->acumularTarjetas(amarillasA, rojasA);
    equipo2->acumularTarjetas(amarillasB, rojasB);
}
void Partido::simular() {

    double lambdaA, lambdaB;
    calcularLambdas(lambdaA, lambdaB);

    goles1 = generarGoles(lambdaA);
    goles2 = generarGoles(lambdaB);

    int amarillasA = 0, rojasA = 0;
    int amarillasB = 0, rojasB = 0;

    short int minutoFinal = simularTiempo(amarillasA, rojasA, amarillasB, rojasB);

    seleccionarJugadores();
    repartirGoles();
    actualizarJugadores();
    actualizarEquipos(amarillasA, rojasA, amarillasB, rojasB);

    cout << "Duracion del partido: " << minutoFinal << " minutos" << endl;
}
bool Partido::empate() const {
    return goles1 == goles2;
}
void Partido::desempatarPorRanking() {

    if (equipo1->getRanking() < equipo2->getRanking()) {
        goles1++;
    } else {
        goles2++;
    }
}
void Partido::mostrarResultado() {
    cout << "Fecha: " << fecha<<endl;
    cout << equipo1->getPais() << " " << goles1
         << " - " << goles2 << " "
         << equipo2->getPais() << endl;

    // =========================
    // GOLEADORES
    // =========================
    cout << "Goleadores " << equipo1->getPais() << ": ";
    for (int i = 0; i < 11; i++) {
        ITERACIONES++;
        if (jugadoresA[i].goles > 0)
            cout << jugadoresA[i].jugador->getNumero() << ";";
    }
    cout << endl;

    cout << "Goleadores " << equipo2->getPais() << ": ";
    for (int i = 0; i < 11; i++) {
        ITERACIONES++;
        if (jugadoresB[i].goles > 0)
            cout << jugadoresB[i].jugador->getNumero() << ";";
    }
    cout << endl;
}
Equipo* Partido::getGanador() {

    if (goles1 > goles2)
        return equipo1;
    else if (goles2 > goles1)
        return equipo2;
    else
        return nullptr; // empate
}
string Partido::getFecha() const {
    return fecha;
}


Partido::~Partido() {
    delete[] jugadoresA;
    delete[] jugadoresB;
}
