#ifndef JUGADOR_H
#define JUGADOR_H
#include <iostream>

using namespace std;

class Jugador
{
    string nombre;
    string apellido;
    short int numero;
    short int partidosJugados;
    short int goles;
    short int minutosJugados;
    short int asistencias;
    short int amarillas;
    short int rojas;
    short int faltas;
    short int golesTorneo;
    short int golesTotales;
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
    short int getGoles() const;

    bool operator==(const Jugador& otro) const;

    string getNombre() const;
    string getApellido() const;
    short int getNumero() const;
    short int getPartidos() const;
    void mostrar() const;
    void sumarGol();
    short int getGolesTorneo() const;
    short int getAmarillas() const { return amarillas; }
    short int getRojas() const { return rojas; }
    short int getFaltas() const { return faltas; }
    short int getMinutos() const { return minutosJugados; }
    short int getAsistencias() const { return asistencias; }
    Jugador& operator=(const Jugador& otro);
    bool operator<(const Jugador& otro) const;
    friend ostream& operator<<(ostream& os, const Jugador& j);

};

#endif // JUGADOR_H
