#include "grupo.h"
#include "utilidades.h"
#include <iostream>

using namespace std;

// Constructor
Grupo::Grupo(char letra) {
    this->letra = letra;
}
Grupo::Grupo() {
    letra = '?';
}
bool Grupo::puedeAgregar(Equipo* e) {

    int contadorUEFA = 0;

    for (int i = 0; i < equipos.tamano(); i++) {

        Equipo* actual = equipos.consultar(i);

        if (actual->getConfederacion() == e->getConfederacion()) {

            if (e->getConfederacion() != "UEFA") {
                return false;
            }

            contadorUEFA++;
        }
        ITERACIONES++;
    }
    if (e->getConfederacion() == "UEFA" && contadorUEFA >= 2) {
        return false;
    }

    return true;
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
        ITERACIONES++;
    }
}
void Grupo::quitarUltimo() {
    if (equipos.tamano() > 0) {
        Equipo* e = equipos.consultar(equipos.tamano() - 1);
        equipos.eliminar(e);
    }
}
// Getter
char Grupo::getLetra() const {
    return letra;
}
short int Grupo::getTamano() const {
    return equipos.tamano();
}

Equipo* Grupo::getEquipo(int i) {
    if (i < 0 || i >= equipos.tamano()) return nullptr;
    return equipos.consultar(i);
}
