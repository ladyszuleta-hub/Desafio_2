#include "Grupo.h"
#include <iostream>

using namespace std;

// Constructor
Grupo::Grupo(char letra) {
    this->letra = letra;
}
Grupo::Grupo() {
    letra = '?';
}

void Grupo::agregarEquipo(Equipo* e) {
    if (equipos.tamano() < 4) {
        equipos.agregar(e, equipos.tamano());
    } else {
        cout << "El grupo ya tiene 4 equipos" << endl;
    }
}

// Mostrar equipos
void Grupo::mostrarEquipos() const {
    cout << "Grupo " << letra << ":" << endl;

    for (int i = 0; i < equipos.tamano(); i++) {
        cout << "- " << equipos.consultar(i)->getPais() << endl;
    }
}

Equipo* Grupo::getEquipo(int i) {
    return equipos.consultar(i);
}
// Getter
char Grupo::getLetra() const {
    return letra;
}
