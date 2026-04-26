#include <iostream>
#include <cstdlib> // rand, srand
#include <ctime>   // time
#include <fstream>
#include "Mundial.h"
#include "utilidades.h"



using namespace std;

int main() {
    try{
    ifstream test("selecciones_clasificadas_mundial.csv");


    if (test.is_open()) {
        cout << "Archivo encontrado" << endl;
    } else {
        throw "Archivo NO encontrado" ;
    }
    srand(time(0));

    Mundial mundial;
    mundial.iniciar();

    }
    catch (const char* e){
        cout << "Error: " << e << endl;
    }
    catch (...){
        cout << "Error desconocido" << endl;
    }

    return 0;
}
