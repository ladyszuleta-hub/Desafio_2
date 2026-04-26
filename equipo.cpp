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
void Equipo::setGrupo(char g) {
    grupo = g;
}

char Equipo::getGrupo() const {
    return grupo;
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
    return golesFavor / 10.0; // normalizar
}

double Equipo::getPromedioGolesContra() const {
    return golesContra / 10.0;
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
Jugador* Equipo::getJugador(int i) {
    return &jugadores[i];
}

int Equipo::getCantidadJugadores() const {
    return cantidadJugadores;
}
void Equipo::sumarAmarilla(string fase, int cantidad) {
    amarillasPorFase[fase] += cantidad;
}

int Equipo::getAmarillasFase(string fase) const {

    auto it = amarillasPorFase.find(fase);

    if (it != amarillasPorFase.end())
        return it->second;

    return 0;
}

int Equipo::getAmarillasTotales() const {

    int total = 0;

    for (auto it = amarillasPorFase.begin();
         it != amarillasPorFase.end(); ++it) {
        total += it->second;
    }

    return total;
}
void Equipo::acumularTarjetas(int amarillas, int rojas) {

    this->amarillasTotales += amarillas;
    this->rojasTotales += rojas;
}
int Equipo::getRojasTotales() const {
    return rojasTotales;
}
