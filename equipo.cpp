#include "Equipo.h"
#include <iostream>

Equipo::Equipo() {
    jugadores = nullptr;
    cantidadJugadores = 0;
    golesFavor = 0;
    golesContra = 0;
}

Equipo::Equipo(string pais, string conf, int ranking, string dt, int gf, int gc) {
    this->pais = pais;
    this->confederacion = conf;
    this->ranking = ranking;
    this->directorTecnico = dt;

    golesFavor = gf;
    golesContra = gc;

    jugadores = nullptr;
    cantidadJugadores = 0;
}

void Equipo::inicializarJugadores() {
    cantidadJugadores = 26;
    jugadores = new Jugador[26];

    for(int i = 0; i < 26; i++) {
        string nombre = "nombre" + to_string(i+1);
        string apellido = "apellido" + to_string(i+1);

        jugadores[i] = Jugador(nombre, apellido, i+1);
    }
}

string Equipo::getPais() const {
    return pais;
}
double Equipo::getPromedioGolesFavor() const {
    return (double)golesFavor;
}

double Equipo::getPromedioGolesContra() const {
    return (double)golesContra;
}
void Equipo::actualizarEstadisticas(int gf, int gc) {
    golesFavor += gf;
    golesContra += gc;
}
int Equipo::getGolesFavor() const {
    return golesFavor;
}

int Equipo::getGolesContra() const {
    return golesContra;
}
int Equipo::getRanking() const {
    return ranking;
}
string Equipo::getConfederacion() const {
    return confederacion;
}
Equipo::~Equipo() {
    delete[] jugadores;
}
