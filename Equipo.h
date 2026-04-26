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
    short int ranking;
    string directorTecnico;

    short int golesFavor;
    short int golesContra;

    Jugador* jugadores;
    short int cantidadJugadores;
    map<string, int> amarillasPorFase;
    short int amarillasTotales = 0;
    short int rojasTotales=0;

public:
    Equipo();
    Equipo(const Equipo& otro);
    Equipo(string pais, string conf, int ranking, string dt, int gf, int gc);
    char grupo;

    void setGrupo(char g);
    char getGrupo() const;

    void inicializarJugadores();

    string getPais() const;
    double getPromedioGolesFavor() const;
    double getPromedioGolesContra() const;
    short int getGolesFavor() const;
    short int getGolesContra() const;
    short int getRanking() const;
    string getConfederacion() const;

    void actualizarEstadisticas(int gf, int gc);
    void actualizarEstadisticas(int gf, int gc, int amarillas, int rojas);

    ~Equipo();
    Jugador* getJugador(int i);
    short int getCantidadJugadores() const;
    void sumarAmarilla(string fase, int cantidad);
    short int getAmarillasFase(string fase) const;
    short int getAmarillasTotales() const;
    void acumularTarjetas(int amarillas, int rojas);
    short int getRojasTotales() const;
    bool operator>(const Equipo& otro) const;
};

#endif
