#include "Equipo.h"
#include <iostream>

    Equipo::Equipo() {
    jugadores = nullptr;
    cantidadJugadores = 0;
    golesFavor = 0;
    golesContra = 0;
}

Equipo::Equipo(string pais, string conf, int ranking, string dt) {
    this->pais = pais;
    this->confederacion = conf;
    this->ranking = ranking;
    this->directorTecnico = dt;

    golesFavor = 0;
    golesContra = 0;

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

void Equipo::actualizarEstadisticas(int gf, int gc) {
    golesFavor += gf;
    golesContra += gc;
}

Equipo::~Equipo() {
    delete[] jugadores;
}
