#include "Sorteo.h"
#include "utilidades.h"
#include <cstdlib>

void Sorteo::ordenarPorRanking(Lista<Equipo*>& equipos) {

    short int n = equipos.tamano();

    for (int i = 0; i < n - 1; i++) {
        ITERACIONES++;
        for (int j = 0; j < n - i - 1; j++) {
            ITERACIONES++;

            Equipo* e1 = equipos.consultar(j);
            Equipo* e2 = equipos.consultar(j + 1);

            if (e1->getRanking() > e2->getRanking()) {
                equipos.reemplazar(e2, j);
                equipos.reemplazar(e1, j + 1);
            }
        }
    }
}

bool* Sorteo::crearArrayUsado(int n) {

    bool* usado = new bool[n];

    for (int i = 0; i < n; i++) {
        ITERACIONES++;
        usado[i] = false;
    }

    return usado;
}

void Sorteo::asignarUSA(Lista<Equipo*>& equipos, Lista<Equipo*>& bombo1, bool* usado) {

    short int n = equipos.tamano();

    for (int i = 0; i < n; i++) {
        ITERACIONES++;

        if (equipos.consultar(i)->getPais() == "United States") {

            bombo1.agregar(equipos.consultar(i), bombo1.tamano());
            usado[i] = true;
            break;
        }
    }
}

void Sorteo::llenarBombo(Lista<Equipo*>& equipos, Lista<Equipo*>& bombo, bool* usado, int limite) {

    short int n = equipos.tamano();

    for (int i = 0; i < n && bombo.tamano() < limite; i++) {
        ITERACIONES++;

        if (!usado[i]) {
            bombo.agregar(equipos.consultar(i), bombo.tamano());
            usado[i] = true;
        }
    }
}

void Sorteo::mezclarLista(Lista<Equipo*>& lista) {

    short int n = lista.tamano();

    for (int i = 0; i < n; i++) {
        ITERACIONES++;

        int j = rand() % n;

        Equipo* a = lista.consultar(i);
        Equipo* b = lista.consultar(j);

        lista.reemplazar(b, i);
        lista.reemplazar(a, j);
    }
}

void Sorteo::liberarUsado(bool* usado) {
    delete[] usado;
}
