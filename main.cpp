#include <iostream>
#include <cstdlib> // rand, srand
#include <ctime>   // time
#include <fstream>
#include <sstream>
#include "Mundial.h"



using namespace std;

int main() {
    try{
    // ya lee el archivo pero sigue con problemas
    ifstream test("selecciones_clasificadas_mundial.csv");


    if (test.is_open()) {
        cout << "Archivo encontrado" << endl;
    } else {
        cout << "Archivo NO encontrado" << endl;
    }
    srand(time(0));

    Mundial mundial;

    cout << "===== CARGANDO EQUIPOS =====" << endl;
    mundial.cargarEquipos("selecciones_clasificadas_mundial.csv");
    mundial.crearBombos();

    cout << "\n===== FORMANDO GRUPOS =====" << endl;
    mundial.formarGrupos();

    cout << "\n===== GRUPOS =====" << endl;
    mundial.mostrarGrupos();

    cout << "\n===== GENERANDO PARTIDOS =====" << endl;
    //mundial.generarPartidosGrupos();

    cout << "\n===== SIMULANDO FASE DE GRUPOS =====" << endl;
    //mundial.simularFaseGrupos();

    cout << "\n===== MEMORIA =====" << endl;
    mundial.mostrarMemoria();

    cout << "\n===== FIN =====" << endl;
    }
    catch (const char* e){
        cout << "Error: " << e << endl;
    }
    catch (...){
        cout << "Error desconocido" << endl;
    }

    return 0;
}
