#include "jugador.h"
#include <iostream>

using namespace std;

int main()
{
    Jugador j1("nombre1", "apellido1", 10);

    j1.jugarPartido(90);
    j1.anotarGol();
    j1.recibirAmarilla();

    j1.mostrar();
    return 0;
}
