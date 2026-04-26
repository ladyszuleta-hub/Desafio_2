#include "Partido.h"
#include <iostream>
#include <cstdlib> // rand()
#include <cmath>
#include <ctime>
#include <string>
using namespace std;

int generarPoisson(double lambda) {
    double L = exp(-lambda);
    int k = 0;
    double p = 1.0;

    do {
        k++;
        p *= (double)rand() / RAND_MAX;
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
void Partido::simular() {

    double mu = 1.35;
    double alpha = 0.6;
    double beta = 0.4;

    double gfA = equipo1->getPromedioGolesFavor();
    double gcA = equipo1->getPromedioGolesContra();

    double gfB = equipo2->getPromedioGolesFavor();
    double gcB = equipo2->getPromedioGolesContra();


    //  CÁLCULO DE LAMBDA

    double lambdaA = mu * pow(gfA / mu, alpha) * pow(gcB / mu, beta);
    double lambdaB = mu * pow(gfB / mu, alpha) * pow(gcA / mu, beta);

    if (lambdaA < 0) lambdaA = 0;
    if (lambdaB < 0) lambdaB = 0;

    if (lambdaA > 5) lambdaA = 5;
    if (lambdaB > 5) lambdaB = 5;


    //  FUNCIÓN POISSON SIMPLE

    auto generarGoles = [](double lambda) {
        int goles = 0;

        for (int i = 0; i < 10; i++) {
            if ((rand() / (double)RAND_MAX) < lambda / 10.0)
                goles++;
        }

        return goles;
    };


    //  GOLES TIEMPO NORMAL (90')

    goles1 = generarGoles(lambdaA);
    goles2 = generarGoles(lambdaB);

    // =========================
    int amarillasA = 0, rojasA = 0;
    int amarillasB = 0, rojasB = 0;


    // SIMULACIÓN 90 MINUTOS

    for (int minuto = 1; minuto <= 90; minuto++) {

        // eventos simples opcionales
        if (rand() % 1000 < 3) amarillasA++;
        if (rand() % 1000 < 3) amarillasB++;

        if (rand() % 2000 < 2) rojasA++;
        if (rand() % 2000 < 2) rojasB++;
    }


    // PRÓRROGA (SI EMPATE)

    int minutoFinal = 90;

    if (goles1 == goles2) {

        for (int minuto = 91; minuto <= 120; minuto++) {

            if (rand() % 1000 < 4) goles1++;
            if (rand() % 1000 < 4) goles2++;

            if (rand() % 1000 < 2) amarillasA++;
            if (rand() % 1000 < 2) amarillasB++;
        }

        minutoFinal = 120;
    }


    //  SELECCIÓN DE JUGADORES

    for (int i = 0; i < 11; i++) {

        int idA = rand() % equipo1->getCantidadJugadores();
        int idB = rand() % equipo2->getCantidadJugadores();

        jugadoresA[i].jugador = equipo1->getJugador(idA);
        jugadoresB[i].jugador = equipo2->getJugador(idB);

        jugadoresA[i].goles = 0;
        jugadoresB[i].goles = 0;

        jugadoresA[i].amarillas = 0;
        jugadoresB[i].amarillas = 0;

        jugadoresA[i].rojas = 0;
        jugadoresB[i].rojas = 0;

        jugadoresA[i].faltas = 0;
        jugadoresB[i].faltas = 0;

        jugadoresA[i].minutos = 90;
        jugadoresB[i].minutos = 90;
    }


    //  DISTRIBUIR GOLES

    for (int i = 0; i < goles1; i++) {
        int j = rand() % 11;
        jugadoresA[j].goles++;
    }

    for (int i = 0; i < goles2; i++) {
        int j = rand() % 11;
        jugadoresB[j].goles++;
    }


    // TARJETAS + FALTAs
    for (int i = 0; i < 11; i++) {

        jugadoresA[i].jugador->jugarPartido(jugadoresA[i].minutos);
        jugadoresB[i].jugador->jugarPartido(jugadoresB[i].minutos);

        for (int g = 0; g < jugadoresA[i].goles; g++)
            jugadoresA[i].jugador->anotarGol();

        for (int g = 0; g < jugadoresB[i].goles; g++)
            jugadoresB[i].jugador->anotarGol();

        if (jugadoresA[i].amarillas)
            jugadoresA[i].jugador->recibirAmarilla();

        if (jugadoresB[i].amarillas)
            jugadoresB[i].jugador->recibirAmarilla();

        if (jugadoresA[i].rojas)
            jugadoresA[i].jugador->recibirRoja();

        if (jugadoresB[i].rojas)
            jugadoresB[i].jugador->recibirRoja();

        jugadoresA[i].jugador->Cometerfalta();
        jugadoresB[i].jugador->Cometerfalta();
    }



    //  ACTUALIZAR JUGADORES

    for (int i = 0; i < 11; i++) {

        jugadoresA[i].jugador->jugarPartido(jugadoresA[i].minutos);
        jugadoresB[i].jugador->jugarPartido(jugadoresB[i].minutos);

        for (int g = 0; g < jugadoresA[i].goles; g++)
            jugadoresA[i].jugador->anotarGol();

        for (int g = 0; g < jugadoresB[i].goles; g++)
            jugadoresB[i].jugador->anotarGol();

        if (jugadoresA[i].amarillas)
            jugadoresA[i].jugador->recibirAmarilla();

        if (jugadoresB[i].amarillas)
            jugadoresB[i].jugador->recibirAmarilla();

        if (jugadoresA[i].rojas)
            jugadoresA[i].jugador->recibirRoja();

        if (jugadoresB[i].rojas)
            jugadoresB[i].jugador->recibirRoja();

        jugadoresA[i].jugador->Cometerfalta();
        jugadoresB[i].jugador->Cometerfalta();
    }


    // ACTUALIZAR EQUIPOS

    equipo1->actualizarEstadisticas(goles1, goles2);
    equipo2->actualizarEstadisticas(goles2, goles1);

    equipo1->acumularTarjetas(amarillasA, rojasA);
    equipo2->acumularTarjetas(amarillasB, rojasB);


    // TIEMPO DEL PARTIDO

    cout << "Duración del partido: " << minutoFinal << " minutos" << endl;


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
    cout << equipo1->getPais() << " " << goles1
         << " - " << goles2 << " "
         << equipo2->getPais() << endl;

    // =========================
    // GOLEADORES
    // =========================
    cout << "Goleadores " << equipo1->getPais() << ": ";
    for (int i = 0; i < 11; i++) {
        if (jugadoresA[i].goles > 0)
            cout << jugadoresA[i].jugador->getNumero() << " ";
    }
    cout << endl;

    cout << "Goleadores " << equipo2->getPais() << ": ";
    for (int i = 0; i < 11; i++) {
        if (jugadoresB[i].goles > 0)
            cout << jugadoresB[i].jugador->getNumero() << " ";
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

Partido::~Partido() {
    delete[] jugadoresA;
    delete[] jugadoresB;
}
