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
public:
    Jugador();
    Jugador(string nombre, string apellido, int numero);
    Jugador(const Jugador& otro);//Constructor de copia.

    // Destructor
    ~Jugador();

    void registrarPartido(int minutos);
    void anotarGol();
    void recibirAmarilla();
    void recibirRoja();
    void Cometerfalta();
    void Asistencias();
    void jugarPartido(int minutos);

    string getNombre() const;
    string getApellido() const;
    int getNumero() const;
    int getGoles() const;
    int getPartidos() const;
    void mostrar() const;
};

#endif // JUGADOR_H
