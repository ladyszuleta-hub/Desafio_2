#include <iostream>
#include "Equipo.h"
#include "partido.h"
#include "Lista.h"

using namespace std;

int main() {

    Lista<Equipo*> listaEquipos;

    Equipo* e1 = new Equipo("Colombia", "CONMEBOL", 10, "DT1");
    Equipo* e2 = new Equipo("Brasil", "CONMEBOL", 1, "DT2");

    e1->inicializarJugadores();
    e2->inicializarJugadores();


    Partido p(e1, e2);

    p.simular();
    p.mostrarResultado();

    return 0;
}
