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
void Mundial::mezclarLista(Lista<Equipo*>& lista) {

    int n = lista.tamano();

    for (int i = 0; i < n; i++) {

        int j = rand() % n;

        Equipo* tempI = lista.consultar(i);
        Equipo* tempJ = lista.consultar(j);

        lista.reemplazar(tempJ, i);
        lista.reemplazar(tempI, j);
    }
}
#include "Sorteo.h"

void Mundial::crearBombos() {

    Sorteo sorteo;

    int n = equipos.tamano();

    sorteo.ordenarPorRanking(equipos);

    bool* usado = sorteo.crearArrayUsado(n);

    sorteo.asignarUSA(equipos, bombo1, usado);

    sorteo.llenarBombo(equipos, bombo1, usado, 12);
    sorteo.llenarBombo(equipos, bombo2, usado, 12);
    sorteo.llenarBombo(equipos, bombo3, usado, 12);
    sorteo.llenarBombo(equipos, bombo4, usado, 12);

    sorteo.mezclarLista(bombo1);
    sorteo.mezclarLista(bombo2);
    sorteo.mezclarLista(bombo3);
    sorteo.mezclarLista(bombo4);

    sorteo.liberarUsado(usado);
}
bool Mundial::asignar(int grupoIdx, int bomboIdx) {

    if (grupoIdx == grupos.tamano()) {
        return true;
    }

    Grupo* g = grupos.consultar(grupoIdx);

    Lista<Equipo*>* bombo;

    if (bomboIdx == 0) bombo = &bombo1;
    else if (bomboIdx == 1) bombo = &bombo2;
    else if (bomboIdx == 2) bombo = &bombo3;
    else bombo = &bombo4;

    for (int i = 0; i < bombo->tamano(); i++) {

        Equipo* e = bombo->consultar(i);

        if (g->puedeAgregar(e)) {

            g->agregarEquipo(e);
            e->setGrupo(g->getLetra());
            bombo->eliminar(e);

            int nextGrupo = grupoIdx;
            int nextBombo = bomboIdx + 1;

            if (nextBombo == 4) {
                nextBombo = 0;
                nextGrupo++;
            }

            if (asignar(nextGrupo, nextBombo)) {
                return true;
            }

            bombo->agregar(e, bombo->tamano());
            g->quitarUltimo();
        }
    }

    return false;
}


void Mundial::formarGrupos() {

    if (!asignar(0, 0)) {
        throw "No se pudo formar grupos (caso extremadamente raro)";
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

        // actualizar tablas inmediatamente
        for (int j = 0; j < tablas.tamano(); j++) {
            tablas.consultar(j)->actualizar(p);
        }
    }
}
void Mundial::mostrarTablas() {

    for (int i = 0; i < tablas.tamano(); i++) {

        //cout << "Grupo " << i << endl;
        tablas.consultar(i)->mostrar();
        cout << endl;
    }
}
void Mundial::crearTablas() {

    for (int i = 0; i < grupos.tamano(); i++) {

        TablaGrupo* t = new TablaGrupo(grupos.consultar(i));

        t->inicializar();

        tablas.agregar(t, tablas.tamano());
    }
}
Lista<Equipo*> Mundial::obtenerClasificados() {

    Lista<Equipo*> clasificados;

    for (int i = 0; i < tablas.tamano(); i++) {

        clasificados.concatenar(
            tablas.consultar(i)->clasificados()
            );
    }

    return clasificados;
}
void Mundial::ordenarTerceros(Lista<StatsEquipo*>& terceros) {

    int n = terceros.tamano();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            StatsEquipo* a = terceros.consultar(j);
            StatsEquipo* b = terceros.consultar(j + 1);

            bool swap = false;

            if (a->puntos < b->puntos) swap = true;

            else if (a->puntos == b->puntos &&
                     (a->gf - a->gc) < (b->gf - b->gc)) swap = true;

            else if (a->puntos == b->puntos &&
                     (a->gf - a->gc) == (b->gf - b->gc) &&
                     a->gf < b->gf) swap = true;

            if (swap) {
                terceros.reemplazar(b, j);
                terceros.reemplazar(a, j + 1);
            }
        }
    }
}
void Mundial::calcularClasificados() {
    clasificadosR16.vaciar();

    Lista<Equipo*> primeros;
    Lista<Equipo*> segundos;
    Lista<StatsEquipo*> terceros;
    for (int i = 0; i < tablas.tamano(); i++) {

        TablaGrupo* t = tablas.consultar(i);
        t->ordenar();

        primeros.agregar(t->get(0)->equipo, primeros.tamano());
        segundos.agregar(t->get(1)->equipo, segundos.tamano());
        terceros.agregar(t->get(2), terceros.tamano());
    }

    ordenarTerceros(terceros);

    clasificadosR16.vaciar();

    for (int i = 0; i < primeros.tamano(); i++)
        clasificadosR16.agregar(primeros.consultar(i), clasificadosR16.tamano());

    for (int i = 0; i < segundos.tamano(); i++)
        clasificadosR16.agregar(segundos.consultar(i), clasificadosR16.tamano());

    for (int i = 0; i < 8; i++)
        clasificadosR16.agregar(terceros.consultar(i)->equipo, clasificadosR16.tamano());
}
Partido* Mundial::crearCruceValido(Lista<Equipo*>& A, Lista<Equipo*>& B) {

    for (int i = 0; i < A.tamano(); i++) {
        for (int j = 0; j < B.tamano(); j++) {

            Equipo* e1 = A.consultar(i);
            Equipo* e2 = B.consultar(j);

            if (e1->getGrupo() != e2->getGrupo()) {

                A.eliminar(e1);
                B.eliminar(e2);

                return new Partido(
                    e1, e2,
                    "10-07-2026",
                    "00:00",
                    "nombreSede",
                    "a1","a2","a3",
                    R16
                    );
            }
        }
    }

    return nullptr;
}
void Mundial::generarR16() {

    Lista<Equipo*> p = primeros;
    Lista<Equipo*> s = segundos;
    Lista<Equipo*> t = terceros;

    while (t.tamano() > 0) {

        Partido* partido = crearCruceValido(p, t);

        if (!partido) {
            throw "Error emparejando primeros vs terceros";
        }

        partidosR16.agregar(partido, partidosR16.tamano());
    }

    while (p.tamano() > 0) {

        Partido* partido = crearCruceValido(p, s);

        if (!partido) {
            throw "Error emparejando primeros vs segundos";
        }

        partidosR16.agregar(partido, partidosR16.tamano());
    }

    while (s.tamano() > 0) {

        Partido* partido = crearCruceValido(s, s);

        if (!partido) {
            throw "Error emparejando segundos entre sí";
        }

        partidosR16.agregar(partido, partidosR16.tamano());
    }
}
void Mundial::simularR16() {

    for (int i = 0; i < partidosR16.tamano(); i++) {

        Partido* p = partidosR16.consultar(i);

        p->simular();

        if (p->empate()) {
            p->desempatarPorRanking();
        }

        p->mostrarResultado();
    }
}
void Mundial::mostrarMemoria() {
    cout << "Memoria usada por listas: "
         << Lista<int>::memoryUsage() << " bytes" << endl;
}
void Mundial::mostrarR16() {

    cout << "\n===== DIECISEISAVOS DE FINAL =====\n" << endl;

    cout << "Equipo A\tGoles\tEquipo B\tGanador\n";

    for (int i = 0; i < partidosR16.tamano(); i++) {

        Partido* p = partidosR16.consultar(i);

        cout << p->getEquipo1()->getPais() << "\t"
             << p->getGoles1() << " - " << p->getGoles2() << "\t"
             << p->getEquipo2()->getPais() << "\t";

        if (p->getGoles1() > p->getGoles2())
            cout << p->getEquipo1()->getPais();
        else
            cout << p->getEquipo2()->getPais();

        cout << endl;
    }
}
void Mundial::iniciar() {

    cargarEquipos("selecciones_clasificadas_mundial.csv");
    crearBombos();
    formarGrupos();
    cout << "\n===== GRUPOS =====" << endl;
    mostrarGrupos();
    crearTablas();
    generarPartidosGrupos();
    simularFaseGrupos();

    cout << "\n===== TABLAS =====" << endl;
    mostrarTablas();
    calcularClasificados();

    generarR16();
    simularR16();
    mostrarR16();
}

Mundial::~Mundial() {

    for (int i = 0; i < equipos.tamano(); i++) {
        delete equipos.consultar(i);
    }

    for (int i = 0; i < partidos.tamano(); i++) {
        delete partidos.consultar(i);
    }
}
