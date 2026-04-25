#include "Sorteo.h"
#include <cstdlib>

void Sorteo::ordenarPorRanking(Lista<Equipo*>& equipos) {

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
}

bool* Sorteo::crearArrayUsado(int n) {

    bool* usado = new bool[n];

    for (int i = 0; i < n; i++) {
        usado[i] = false;
    }

    return usado;
}

void Sorteo::asignarUSA(Lista<Equipo*>& equipos, Lista<Equipo*>& bombo1, bool* usado) {

    int n = equipos.tamano();

    for (int i = 0; i < n; i++) {

        if (equipos.consultar(i)->getPais() == "United States") {

            bombo1.agregar(equipos.consultar(i), bombo1.tamano());
            usado[i] = true;
            break;
        }
    }
}

void Sorteo::llenarBombo(Lista<Equipo*>& equipos, Lista<Equipo*>& bombo, bool* usado, int limite) {

    int n = equipos.tamano();

    for (int i = 0; i < n && bombo.tamano() < limite; i++) {

        if (!usado[i]) {
            bombo.agregar(equipos.consultar(i), bombo.tamano());
            usado[i] = true;
        }
    }
}

void Sorteo::mezclarLista(Lista<Equipo*>& lista) {

    int n = lista.tamano();

    for (int i = 0; i < n; i++) {

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
