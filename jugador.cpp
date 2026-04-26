#include "jugador.h"

Jugador::Jugador() {
    nombre = "";
    apellido = "";
    numero = 0;
    partidosJugados = 0;
    goles = 0;
    minutosJugados = 0;
    asistencias = 0;
    amarillas = 0;
    rojas = 0;
    faltas = 0;
}
Jugador::Jugador(string nombre, string apellido, int numero) {
    this->nombre = nombre;
    this->apellido = apellido;
    this->numero = numero;

    partidosJugados = 0;
    goles = 0;
    minutosJugados = 0;
    asistencias = 0;
    amarillas = 0;
    rojas = 0;
    faltas = 0;
}
Jugador::Jugador(const Jugador& otro) {
    nombre = otro.nombre;
    apellido = otro.apellido;
    numero = otro.numero;

    partidosJugados = otro.partidosJugados;
    goles = otro.goles;
    minutosJugados = otro.minutosJugados;
    asistencias = otro.asistencias;
    amarillas = otro.amarillas;
    rojas = otro.rojas;
    faltas = otro.faltas;
}
Jugador::~Jugador() {}
void Jugador::jugarPartido(int minutos) {
    partidosJugados++;
    minutosJugados += minutos;
}

string Jugador::getNombre() const { return nombre; }
string Jugador::getApellido() const { return apellido; }
int Jugador::getNumero() const { return numero; }
int Jugador::getGoles() const { return goles; }
int Jugador::getPartidos() const { return partidosJugados; }
void Jugador::anotarGol() {
    goles++;
}

void Jugador::Asistencias() {
    asistencias++;
}

void Jugador::recibirAmarilla() {
    amarillas++;
}

void Jugador::recibirRoja() {
    rojas++;
}
void Jugador::Cometerfalta(){
    faltas++;
}
bool Jugador::operator==(const Jugador& otro) const {
    return numero == otro.numero;
}

void Jugador::mostrar() const {
    cout << "Jugador: " << nombre << " " << apellido << endl;
    cout << "Numero: " << numero << endl;
    cout << "Goles: " << goles << endl;
    cout << "Partidos: " << partidosJugados << endl;
    cout << "Minutos: " << minutosJugados << endl;
    cout << "Amarillas: " << amarillas << endl;
    cout << "Rojas: " << rojas << endl;
    cout << "Faltas: " << faltas << endl;
}
void Jugador::sumarGol() {
    golesTorneo++;
}

int Jugador::getGolesTorneo() const {
    return golesTorneo;
}

