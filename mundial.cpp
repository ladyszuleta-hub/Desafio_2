#include "Mundial.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
Mundial::Mundial() {
    for (int i = 0; i < 12; i++) {
        grupos.agregar(Grupo('A' + i), i);
    }
}
void Mundial::cargarEquipos(const string& archivo) {
    ifstream file(archivo);

    if (!file.is_open()) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    string linea;

    // Saltar encabezado
    getline(file, linea);

    int i = 0;
    while (getline(file, linea)) {
        stringstream ss(linea);

        string pais, conf, rankingStr, dt;

        getline(ss, pais, ',');
        getline(ss, conf, ',');
        getline(ss, rankingStr, ',');
        getline(ss, dt, ',');

        int ranking = stoi(rankingStr);

        Equipo* e = new Equipo(pais, conf, ranking, dt);
        e->inicializarJugadores();

        equipos.agregar(e, i++);
    }

    file.close();
}
void Mundial::formarGrupos() {

    // Mezclar equipos (Fisher-Yates simple)
    int n = equipos.tamano();

    for (int i = 0; i < n; i++) {
        int j = rand() % n;

        Equipo* temp = equipos.consultar(i);
        equipos.reemplazar(equipos.consultar(j), i);
        equipos.reemplazar(temp, j);
    }

    // Repartir en grupos
    int index = 0;

    for (int i = 0; i < grupos.tamano(); i++) {
        Grupo& g = grupos.consultar(i);

        for (int j = 0; j < 4; j++) {
            g.agregarEquipo(equipos.consultar(index++));
        }
    }
}
void Mundial::mostrarGrupos() {
    for (int i = 0; i < grupos.tamano(); i++) {
        grupos.consultar(i).mostrarEquipos();
        cout << endl;
    }
}
void Mundial::generarPartidosGrupos() {

    for (int i = 0; i < grupos.tamano(); i++) {

        Grupo& g = grupos.consultar(i);

        Equipo* e[4];

        for (int j = 0; j < 4; j++) {
            e[j] = g.getEquipo(j);
        }

        // todos contra todos
        int comb[6][2] = {
            {0,1}, {0,2}, {0,3},
            {1,2}, {1,3}, {2,3}
        };

        for (int k = 0; k < 6; k++) {
            Partido* p = new Partido(e[comb[k][0]], e[comb[k][1]]);
            partidos.agregar(p, partidos.tamano());
        }
    }
}
void Mundial::simularFaseGrupos() {

    for (int i = 0; i < partidos.tamano(); i++) {
        Partido* p = partidos.consultar(i);

        p->simular();
        p->mostrarResultado();
    }
}
void Mundial::mostrarMemoria() {
    cout << "Memoria usada por listas: "
         << Lista<int>::memoryUsage() << " bytes" << endl;
}
