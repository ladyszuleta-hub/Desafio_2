#ifndef EQUIPO_H
#define EQUIPO_H

#include <string>
using namespace std;

#include "Jugador.h"

class Equipo {
private:
    string pais;
    string confederacion;
    int ranking;
    string directorTecnico;

    int golesFavor;
    int golesContra;

    Jugador* jugadores;
    int cantidadJugadores;

public:
    Equipo();
    Equipo(string pais, string conf, int ranking, string dt);

    void inicializarJugadores();

    string getPais() const;

    void actualizarEstadisticas(int gf, int gc);

    ~Equipo();
};

#endif
