#include "mundial.h"
#include "partido.h"
#include "sorteo.h"
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
string Mundial::calcularFecha(string /*base*/, int dias) {
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

        if (p == nullptr) continue;

        p->simular();
        p->mostrarResultado();

        if (tablas.tamano() == 0) {
            cout << "ERROR: tablas vacias" << endl;
            continue;
        }

        for (int j = 0; j < tablas.tamano(); j++) {

            if (tablas.consultar(j) != nullptr) {
                tablas.consultar(j)->actualizar(p);
            }
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

    primeros.vaciar();
    segundos.vaciar();
    terceros.vaciar();
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

            if (e1 == nullptr || e2 == nullptr) continue;

            if (!yaSeEnfrentaron(e1, e2)) {

                registrarEnfrentamiento(e1, e2);

                return new Partido(
                    e1,
                    e2,
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
    Lista<Equipo*> t;

    int limite = (terceros.tamano() < 8) ? terceros.tamano() : 8;

    for (int i = 0; i < limite; i++) {
        t.agregar(terceros.consultar(i)->equipo, t.tamano());
    }

    // P vs T
    while (!p.esVacia() && !t.esVacia()) {

        Partido* partido = crearCruceValido(p, t);

        if (!partido) break;

        partidosR16.agregar(partido, partidosR16.tamano());

        p.eliminar(partido->getEquipo1());
        t.eliminar(partido->getEquipo2());
    }

    // P vs S
    while (!p.esVacia() && !s.esVacia()) {

        Partido* partido = crearCruceValido(p, s);

        if (!partido) break;

        partidosR16.agregar(partido, partidosR16.tamano());

        p.eliminar(partido->getEquipo1());
        s.eliminar(partido->getEquipo2());
    }

    // S vs S
    while (s.tamano() > 1) {

        Partido* partido = crearCruceValido(s, s);

        if (!partido) break;

        partidosR16.agregar(partido, partidosR16.tamano());

        s.eliminar(partido->getEquipo1());
        s.eliminar(partido->getEquipo2());
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

    size_t memoria = 0;

    memoria += equipos.tamano() * sizeof(Equipo*);
    memoria += partidos.tamano() * sizeof(Partido*);
    memoria += clasificadosR16.tamano() * sizeof(Equipo*);

    cout << "\n🧠 Memoria estimada: " << memoria << " bytes\n";
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

    // =========================
    // DIECISEISAVOS (R16)
    // =========================
    cout << "\n===== DIECISEISAVOS =====" << endl;

    generarR16();
    Lista<Equipo*> clasificados8;
    simularFase(partidosR16, clasificados8);

    // =========================
    // OCTAVOS DE FINAL
    // =========================
    cout << "\n===== OCTAVOS DE FINAL =====" << endl;

    Lista<Partido*> octavos = generarFase(clasificados8, R16);
    Lista<Equipo*> clasificados4;

    simularFase(octavos, clasificados4);

    // =========================
    // CUARTOS DE FINAL
    // =========================
    cout << "\n===== CUARTOS DE FINAL =====" << endl;

    Lista<Partido*> cuartos = generarFase(clasificados4, R16);
    Lista<Equipo*> clasificados2;

    simularFase(cuartos, clasificados2);

    // =========================
    // SEMIFINALES
    // =========================
    cout << "\n===== SEMIFINALES =====" << endl;

    Lista<Partido*> semifinal = generarFase(clasificados2, R16);
    Lista<Equipo*> finalistas;

    simularFase(semifinal, finalistas);

    // =========================
    // FINAL
    // =========================
    cout << "\n===== FINAL =====" << endl;

    Lista<Partido*> final = generarFase(finalistas, R16);
    Lista<Equipo*> campeonLista;

    simularFase(final, campeonLista);

    campeon = campeonLista.consultar(0);

    // =========================
    // CAMPEÓN
    // =========================
    cout << "\n===== CAMPEÓN =====" << endl;
   cout << campeon->getPais() << endl;

    // =========================
    // ESTADÍSTICAS FINALES
    // =========================
    cout << "\n===== ESTADÍSTICAS DEL MUNDIAL =====" << endl;

    mostrarEstadisticasFinales();
}
Mundial::~Mundial() {
    for (int i = 0; i < partidosR16.tamano(); i++) {
        delete partidosR16.consultar(i);
    }
}
void Mundial::registrarEnfrentamiento(Equipo* a, Equipo* b) {

    Enfrentamiento e;
    e.a = a;
    e.b = b;

    historial.agregar(e, historial.tamano());
}
bool Mundial::yaSeEnfrentaron(Equipo* a, Equipo* b) {

    for (int i = 0; i < historial.tamano(); i++) {

        Enfrentamiento e = historial.consultar(i);

        if ((e.a == a && e.b == b) ||
            (e.a == b && e.b == a)) {
            return true;
        }
    }

    return false;
}
Lista<Partido*> Mundial::generarFase(Lista<Equipo*>& equipos, Fase f) {

    Lista<Partido*> fase;

    int i = 0;

    while (i + 1 < equipos.tamano()) {

        Equipo* a = equipos.consultar(i);
        Equipo* b = equipos.consultar(i + 1);

        if (!yaSeEnfrentaron(a, b)) {

            registrarEnfrentamiento(a, b);

            Partido* p = new Partido(
                a, b,
                "10-07-2026",
                "00:00",
                "nombreSede",
                "a1","a2","a3",
                f
                );

            fase.agregar(p, fase.tamano());
        }

        i += 2;
    }

    return fase;
}
void Mundial::simularFase(Lista<Partido*>& fase, Lista<Equipo*>& clasificados) {

    clasificados.vaciar();

    for (int i = 0; i < fase.tamano(); i++) {

        Partido* p = fase.consultar(i);

        p->simular();

        if (p->empate()) {
            p->desempatarPorRanking();
        }

        p->mostrarResultado();

        Equipo* ganador = p->getGanador();

        if (ganador != nullptr) {
            clasificados.agregar(ganador, clasificados.tamano());
        }
    }
}
void Mundial::mostrarTop4() {

    Lista<Equipo*> todos = clasificadosR16;

    for (int i = 0; i < todos.tamano(); i++) {
        for (int j = 0; j < todos.tamano() - 1; j++) {

            Equipo* a = todos.consultar(j);
            Equipo* b = todos.consultar(j + 1);

            // orden por goles del mundial (lo que sí existe)
            if (a->getGolesFavor() < b->getGolesFavor()) {
                todos.reemplazar(b, j);
                todos.reemplazar(a, j + 1);
            }
        }
    }

    cout << "\n TOP 4 DEL MUNDIAL:\n";

    for (int i = 0; i < 4 && i < todos.tamano(); i++) {
        cout << i + 1 << ". " << todos.consultar(i)->getPais()
        << " (" << todos.consultar(i)->getGolesFavor() << " goles)\n";
    }
}


void Mundial::top3Goleadores() {

    Lista<Jugador*> jugadores;

    // RECOLECTAR TODOS LOS JUGADORES

    for (int i = 0; i < equipos.tamano(); i++) {

        Equipo* e = equipos.consultar(i);

        for (int j = 0; j < e->getCantidadJugadores(); j++) {

            jugadores.agregar(e->getJugador(j), jugadores.tamano());
        }
    }

    if (jugadores.tamano() == 0) {
        cout << "\nNo hay jugadores.\n";
        return;
    }

    // ORDENAR POR GOLES

    for (int i = 0; i < jugadores.tamano() - 1; i++) {
        for (int j = 0; j < jugadores.tamano() - i - 1; j++) {

            Jugador* a = jugadores.consultar(j);
            Jugador* b = jugadores.consultar(j + 1);

            if (a->getGoles() < b->getGoles()) {

                jugadores.reemplazar(b, j);
                jugadores.reemplazar(a, j + 1);
            }
        }
    }

    // MOSTRAR TOP 3

    cout << "\n TOP 3 GOLEADORES DEL MUNDIAL:\n";

    int limite = (jugadores.tamano() < 3) ? jugadores.tamano() : 3;

    for (int i = 0; i < limite; i++) {

        Jugador* j = jugadores.consultar(i);

        cout << i + 1 << ". "
             << j->getNombre() << " "
             << j->getApellido()
             << " - " << j->getGoles() << " goles\n";
    }
}

void Mundial::mostrarEstadisticasFinales() {

    cout << "\n==============================\n";
    cout << ""
            ""
            " INFORME FINAL DEL MUNDIAL\n";
    cout << "==============================\n";


    // TOP 4 EQUIPOS (por goles en el torneo)

    mostrarTop4();

    //  MÁXIMO GOLEADOR DEL CAMPEÓN

   Equipo* campeon = this->campeon;

    Jugador* mejor = nullptr;
    int maxGoles = 0;

    for (int i = 0; i < campeon->getCantidadJugadores(); i++) {

        Jugador* j = campeon->getJugador(i);

        if (j->getGoles() > maxGoles) {
            maxGoles = j->getGoles();
            mejor = j;
        }
    }
    cout << "\n TOTAL TARJETAS DEL CAMPEON:\n";
    cout << campeon->getPais()
         << " - Amarillas: " << campeon->getAmarillasTotales()
         << " - Rojas: " << campeon->getRojasTotales() << endl;

    cout << "\n MAXIMO GOLEADOR DEL CAMPEON:\n";
    cout << mejor->getNombre() << " " << mejor->getApellido()
         << " (" << maxGoles << " goles)\n";

    cout << "\n MAXIMO GOLEADOR DEL CAMPEON:\n";
    if (mejor != nullptr) {
        cout << mejor->getNombre() << " " << mejor->getApellido()
        << " (" << maxGoles << " goles)\n";
    }

    // TOP 3 GOLEADORES DEL MUNDIAL

    top3Goleadores();
    // EQUIPO MÁS GOLEADOR

    Equipo* mejorEquipo = nullptr;
    int maxE = -1;

    for (int i = 0; i < equipos.tamano(); i++) {

        Equipo* e = equipos.consultar(i);

        if (e->getGolesFavor() > maxE) {
            maxE = e->getGolesFavor();
            mejorEquipo = e;
        }
    }

    cout << "\n EQUIPO MAS GOLEADOR:\n";
    cout << mejorEquipo->getPais()
         << " (" << maxE << " goles)\n";


    // MEMORIA

    cout << "\n METRICAS:\n";

    mostrarMemoria();
}
