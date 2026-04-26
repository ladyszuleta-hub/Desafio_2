#ifndef JUGADOR_H
#define JUGADOR_H
#include <iostream>
using namespace std;

class Jugador
{
    string nombre;
    string apellido;
    int numero;
    int partidosJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int amarillas;
    int rojas;
    int faltas;
    int golesTorneo;
    int golesTotales;
public:
    Jugador();
    Jugador(string nombre, string apellido, int numero);
    Jugador(const Jugador& otro);//Constructor de copia.

    // Destructor
    ~Jugador();

    void registrarPartido(int minutos);
    void recibirAmarilla();
    void recibirRoja();
    void Cometerfalta();
    void Asistencias();
    void jugarPartido(int minutos);
    void anotarGol();
    int getGoles() const;

    bool operator==(const Jugador& otro) const;

    string getNombre() const;
    string getApellido() const;
    int getNumero() const;
    int getPartidos() const;
    void mostrar() const;
    void sumarGol();
    int getGolesTorneo() const;

};

#endif // JUGADOR_H
