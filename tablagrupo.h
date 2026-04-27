#ifndef TABLAGRUPOS_H
#define TABLAGRUPOS_H

#include "Grupo.h"
#include "Partido.h"

struct StatsEquipo {
    Equipo* equipo;
    int puntos;
    int gf;
    int gc;
};

class TablaGrupo {
private:
    Grupo* grupo;
    Lista<StatsEquipo*> tabla;

public:
    TablaGrupo();
    TablaGrupo(Grupo* g);

    void inicializar();
    void actualizar(Partido* p);

    void ordenar();

    void mostrar();
    StatsEquipo* get(int i);

    Lista<Equipo*> clasificados();
    ~TablaGrupo();

};


#endif // TABLAGRUPO_H
