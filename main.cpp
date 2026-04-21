#include <iostream>
#include "Equipo.h"
#include "Lista.h"

using namespace std;

int main() {

    Lista<Equipo*> listaEquipos;

    Equipo* e1 = new Equipo("Colombia", "CONMEBOL", 10, "DT1");
    Equipo* e2 = new Equipo("Brasil", "CONMEBOL", 1, "DT2");

    e1->inicializarJugadores();
    e2->inicializarJugadores();

    listaEquipos.agregar(e1, 0);
    listaEquipos.agregar(e2, 1);

    for(int i = 0; i < listaEquipos.tamano(); i++) {
        Equipo* e = listaEquipos.consultar(i);
        cout << "Equipo: " << e->getPais() << endl;
    }

    return 0;
}
