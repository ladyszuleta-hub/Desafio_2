#include "Mundial.h"
#include "partido.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
Mundial::Mundial() {
    for (int i = 0; i < 12; i++) {
        grupos.agregar(new Grupo('A' + i), i);
    }
}
void Mundial::cargarEquipos(const string& archivo) {

    ifstream file(archivo);

    if (!file.is_open()) {
        throw "Error al abrir el archivo";

    }

    string linea;

    // Saltar encabezado
    getline(file, linea);
    getline(file,linea);

    int i = 0;
    while (getline(file, linea)) {

        if (linea.empty()) continue;

        //  Eliminar caracter raro del inicio
        if (linea[0] == '\xEF') {
            linea = linea.substr(3);}

        stringstream ss(linea);

        string pais, conf, rankingStr, dt, federacion, gfStr, gcStr;

        getline(ss,rankingStr,';');
        getline(ss, pais, ';');
        getline(ss, dt, ';');
        getline(ss, federacion, ';');
        getline(ss, conf, ';');
        getline(ss, gfStr, ';');
        getline(ss, gcStr, ';');

        if (rankingStr.empty() || gfStr.empty() || gcStr.empty()) {
            cout << "Error en linea: " << linea << endl;
            continue;
        }

        try {
            int ranking = stoi(rankingStr);
            int gf = stoi(gfStr);
            int gc = stoi(gcStr);

        Equipo* e = new Equipo(pais, conf, ranking, dt, gf, gc);
        e->inicializarJugadores();

        equipos.agregar(e, i++);
    } catch (...) {
            cout << "Error convirtiendo linea: " << linea << endl;
        }
    }

    file.close();
}
void Mundial::crearBombos() {


    int n = equipos.tamano();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            Equipo* e1 = equipos.consultar(j);
            Equipo* e2 = equipos.consultar(j + 1);

            if (e1->getRanking() > e2->getRanking()) {
                equipos.reemplazar(e2, j);
                equipos.reemplazar(e1, j + 1);
            }
        }
    }

    bool usado[48];
    for (int i = 0; i < 48; i++) {
        usado[i] = false;
    }

    for (int i = 0; i < n; i++) {
        if (equipos.consultar(i)->getPais() == "United States") {
            bombo1.agregar(equipos.consultar(i), bombo1.tamano());
            usado[i] = true;
            break;
        }
    }

    for (int i = 0; i < n && bombo1.tamano() < 12; i++) {
        if (!usado[i]) {
            bombo1.agregar(equipos.consultar(i), bombo1.tamano());
            usado[i] = true;
        }
    }

    for (int i = 0; i < n && bombo2.tamano() < 12; i++) {
        if (!usado[i]) {
            bombo2.agregar(equipos.consultar(i), bombo2.tamano());
            usado[i] = true;
        }
    }

    for (int i = 0; i < n && bombo3.tamano() < 12; i++) {
        if (!usado[i]) {
            bombo3.agregar(equipos.consultar(i), bombo3.tamano());
            usado[i] = true;
        }
    }

    for (int i = 0; i < n && bombo4.tamano() < 12; i++) {
        if (!usado[i]) {
            bombo4.agregar(equipos.consultar(i), bombo4.tamano());
            usado[i] = true;
        }
    }
}
Equipo* Mundial::sacarValido(Lista<Equipo*>& bombo, Grupo& g) {

    Lista<Equipo*> validos;

    for (int i = 0; i < bombo.tamano(); i++) {
        Equipo* e = bombo.consultar(i);

        if (g.puedeAgregar(e)) {
            validos.agregar(e, validos.tamano());
        }
    }

    if (validos.tamano() == 0) {
        return NULL;
    }

    int pos = rand() % validos.tamano();
    Equipo* elegido = validos.consultar(pos);

    bombo.eliminar(elegido);

    return elegido;
}

void Mundial::formarGrupos() {


    for (int i = 0; i < grupos.tamano(); i++) {

        Grupo* g = grupos.consultar(i);

        g->agregarEquipo(sacarValido(bombo1, *g));
        g->agregarEquipo(sacarValido(bombo2, *g));
        g->agregarEquipo(sacarValido(bombo3, *g));
        g->agregarEquipo(sacarValido(bombo4, *g));
    }
}
void Mundial::mostrarGrupos() {
    for (int i = 0; i < grupos.tamano(); i++) {
        grupos.consultar(i)->mostrarEquipos();
        cout << endl;
    }
}
string Mundial::calcularFecha(string base, int dias) {
    int dia = 20 + dias;
    return "2026-06-" + to_string(dia);
}
void Mundial::generarPartidosGrupos() {

    int dia = 0;
    int partidosEnDia = 0;

    string fechaBase = "2026-06-20";

    for (int i = 0; i < grupos.tamano(); i++) {

        Grupo* g = grupos.consultar(i);

        Equipo* e[4];
        for (int j = 0; j < 4; j++) {
            e[j] = g->getEquipo(j);
        }

        int comb[6][2] = {
            {0,1}, {0,2}, {0,3},
            {1,2}, {1,3}, {2,3}
        };

        for (int k = 0; k < 6; k++) {

            if (partidosEnDia == 4) {
                dia++;
                partidosEnDia = 0;
            }

            string fecha = calcularFecha("2026-06-20", dia);

            Partido* p = new Partido(
                e[comb[k][0]],
                e[comb[k][1]],
                fecha,
                "00:00",
                "nombreSede",
                "codArbitro1",
                "codArbitro2",
                "codArbitro3",
                GRUPOS
                );

            partidos.agregar(p, partidos.tamano());

            partidosEnDia++;
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

Mundial::~Mundial() {

    for (int i = 0; i < equipos.tamano(); i++) {
        delete equipos.consultar(i);
    }

    for (int i = 0; i < partidos.tamano(); i++) {
        delete partidos.consultar(i);
    }
}
