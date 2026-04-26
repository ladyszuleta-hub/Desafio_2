#ifndef EQUIPO_H
#define EQUIPO_H
#include "jugador.h"
#include <string>
#include <map>
using namespace std;


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
    map<string, int> amarillasPorFase;
    int amarillasTotales = 0;
    int rojasTotales=0;

public:
    Equipo();
    Equipo(string pais, string conf, int ranking, string dt, int gf, int gc);
    char grupo;

    void setGrupo(char g);
    char getGrupo() const;

    void inicializarJugadores();

    string getPais() const;
    double getPromedioGolesFavor() const;
    double getPromedioGolesContra() const;
    int getGolesFavor() const;
    int getGolesContra() const;
    int getRanking() const;
    string getConfederacion() const;

    void actualizarEstadisticas(int gf, int gc);

    ~Equipo();
    Jugador* getJugador(int i);
    int getCantidadJugadores() const;
    void sumarAmarilla(string fase, int cantidad);
    int getAmarillasFase(string fase) const;
    int getAmarillasTotales() const;
    void acumularTarjetas(int amarillas, int rojas);
    int getRojasTotales() const;
};

#endif
