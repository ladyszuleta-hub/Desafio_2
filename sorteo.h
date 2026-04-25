#ifndef SORTEO_H
#define SORTEO_H

#include "Lista.h"
#include "Equipo.h"

class Sorteo {
public:
    void ordenarPorRanking(Lista<Equipo*>& equipos);

    bool* crearArrayUsado(int n);

    void asignarUSA(Lista<Equipo*>& equipos, Lista<Equipo*>& bombo1, bool* usado);

    void llenarBombo(Lista<Equipo*>& equipos, Lista<Equipo*>& bombo, bool* usado, int limite);

    void mezclarLista(Lista<Equipo*>& lista);

    void liberarUsado(bool* usado);
};


#endif // SORTEO_H
