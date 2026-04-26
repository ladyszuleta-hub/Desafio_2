#include "mundial.h"
#include "partido.h"
#include "sorteo.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
Mundial::Mundial() {
    for (int i = 0; i < 12; i++) {
        grupos.agregar(new Grupo('A' + i), i);
        ITERACIONES++;
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
    ITERACIONES++;
    }
    totalEquiposControl = equipos.tamano();

    equiposControl = new Equipo*[totalEquiposControl];
    ultimoDia = new int[totalEquiposControl];

    for (int i = 0; i < totalEquiposControl; i++) {
        ITERACIONES++;
        equiposControl[i] = equipos.consultar(i);
        ultimoDia[i] = -100; // nunca ha jugado
    }

    file.close();
}
int Mundial::buscarEquipoIndex(Equipo* e) {

    for (int i = 0; i < totalEquiposControl; i++) {
        if (equiposControl[i] == e)
            return i;
    }

    return -1;
}
bool Mundial::puedeJugar(Equipo* e, int diaActual) {

    int idx = buscarEquipoIndex(e);

    if (idx == -1) return true;

    return (diaActual - ultimoDia[idx]) >= 3;
}
void Mundial::registrarPartido(Equipo* e1, Equipo* e2, int dia) {

    int i1 = buscarEquipoIndex(e1);
    int i2 = buscarEquipoIndex(e2);

    if (i1 != -1) ultimoDia[i1] = dia;
    if (i2 != -1) ultimoDia[i2] = dia;
}
void Mundial::liberarPartidos(Lista<Partido*>& lista) {

    for (int i = 0; i < lista.tamano(); i++) {
        ITERACIONES++;
        delete lista.consultar(i);
    }

    lista.vaciar();
}
void Mundial::mezclarLista(Lista<Equipo*>& lista) {

    int n = lista.tamano();

    for (int i = 0; i < n; i++) {

        int j = rand() % n;

        Equipo* tempI = lista.consultar(i);
        Equipo* tempJ = lista.consultar(j);

        lista.reemplazar(tempJ, i);
        lista.reemplazar(tempI, j);
        ITERACIONES++;
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
            ITERACIONES++;
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
        ITERACIONES++;
    }
}
string Mundial::calcularFecha(string , int dias) {

    int dia = 20 + dias;
    int mes = 6; // junio

    if (dia > 30) {
        dia -= 30;
        mes = 7; // julio
    }

    string diaStr = (dia < 10) ? "0" + to_string(dia) : to_string(dia);
    string mesStr = (mes < 10) ? "0" + to_string(mes) : to_string(mes);

    return "2026-" + mesStr + "-" + diaStr;
}
void Mundial::generarPartidosGrupos() {

    const int MAXP = 500;

    Equipo* local[MAXP];
    Equipo* visitante[MAXP];
    bool jugado[MAXP];

    int total = 0;

    // Crear todos los partidos pendientes de todos los grupos
    for (int i = 0; i < grupos.tamano(); i++) {
        ITERACIONES++;

        Grupo* g = grupos.consultar(i);

        Equipo* e[4];
        for (int j = 0; j < 4; j++){
            ITERACIONES++;
            e[j] = g->getEquipo(j);}

        int comb[6][2] = {
            {0,1}, {0,2}, {0,3},
            {1,2}, {1,3}, {2,3}
        };

        for (int k = 0; k < 6; k++) {
            ITERACIONES++;
            local[total] = e[comb[k][0]];
            visitante[total] = e[comb[k][1]];
            jugado[total] = false;
            total++;
        }
    }

    int pendientes = total;
    int dia = 0;

    while (pendientes > 0) {
        ITERACIONES++;

        int partidosHoy = 0;

        for (int i = 0; i < total && partidosHoy < 4; i++) {
            ITERACIONES++;
            if (jugado[i]) continue;

            if (puedeJugar(local[i], dia) &&
                puedeJugar(visitante[i], dia)) {

                string fecha = calcularFecha("2026-06-20", dia);

                Partido* p = new Partido(
                    local[i],
                    visitante[i],
                    fecha,
                    "00:00",
                    "nombreSede",
                    "codArbitro1",
                    "codArbitro2",
                    "codArbitro3",
                    GRUPOS
                    );

                partidos.agregar(p, partidos.tamano());

                registrarPartido(local[i], visitante[i], dia);

                jugado[i] = true;
                pendientes--;
                partidosHoy++;
            }
        }

        dia++;
    }
}
void Mundial::simularFaseGrupos() {

    for (int i = 0; i < partidos.tamano(); i++) {
        ITERACIONES++;

        Partido* p = partidos.consultar(i);

        if (p == nullptr) continue;

        p->simular();
        p->mostrarResultado();

        if (tablas.tamano() == 0) {
            cout << "ERROR: tablas vacias" << endl;
            continue;
        }

        for (int j = 0; j < tablas.tamano(); j++) {
            ITERACIONES++;

            if (tablas.consultar(j) != nullptr) {
                tablas.consultar(j)->actualizar(p);
            }
        }
    }
}
void Mundial::mostrarTablas() {

    for (int i = 0; i < tablas.tamano(); i++) {
        ITERACIONES++;

        //cout << "Grupo " << i << endl;
        tablas.consultar(i)->mostrar();
        cout << endl;
    }
}
void Mundial::crearTablas() {

    for (int i = 0; i < grupos.tamano(); i++) {
        ITERACIONES++;

        TablaGrupo* t = new TablaGrupo(grupos.consultar(i));

        t->inicializar();

        tablas.agregar(t, tablas.tamano());
    }
}
Lista<Equipo*> Mundial::obtenerClasificados() {

    Lista<Equipo*> clasificados;

    for (int i = 0; i < tablas.tamano(); i++) {
        ITERACIONES++;

        clasificados.concatenar(
            tablas.consultar(i)->clasificados()
            );
    }

    return clasificados;
}
void Mundial::ordenarTerceros(Lista<StatsEquipo*>& terceros) {

    int n = terceros.tamano();

    for (int i = 0; i < n - 1; i++) {
        ITERACIONES++;
        for (int j = 0; j < n - i - 1; j++) {
            ITERACIONES++;

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
        ITERACIONES++;

        TablaGrupo* t = tablas.consultar(i);
        t->ordenar();

        primeros.agregar(t->get(0)->equipo, primeros.tamano());
        segundos.agregar(t->get(1)->equipo, segundos.tamano());
        terceros.agregar(t->get(2), terceros.tamano());
    }

    ordenarTerceros(terceros);

    clasificadosR16.vaciar();

    for (int i = 0; i < primeros.tamano(); i++){
        ITERACIONES++;
        clasificadosR16.agregar(primeros.consultar(i), clasificadosR16.tamano());}

    for (int i = 0; i < segundos.tamano(); i++){
        ITERACIONES++;
        clasificadosR16.agregar(segundos.consultar(i), clasificadosR16.tamano());}

    for (int i = 0; i < 8; i++){
        ITERACIONES++;
        clasificadosR16.agregar(terceros.consultar(i)->equipo, clasificadosR16.tamano());}
}
Partido* Mundial::crearCruceValido(Lista<Equipo*>& A, Lista<Equipo*>& B) {


    for (int i = 0; i < A.tamano(); i++) {
        ITERACIONES++;
        for (int j = 0; j < B.tamano(); j++) {
            ITERACIONES++;

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
        ITERACIONES++;
        t.agregar(terceros.consultar(i)->equipo, t.tamano());
    }
    // P vs T
    while (!p.esVacia() && !t.esVacia()) {
        ITERACIONES++;

        Partido* partido = crearCruceValido(p, t);

        if (partido == nullptr) {
            break;
        }

        partidosR16.agregar(partido, partidosR16.tamano());

        p.eliminar(partido->getEquipo1());
        t.eliminar(partido->getEquipo2());
    }

    // P vs S
    while (!p.esVacia() && !s.esVacia()) {
        ITERACIONES++;
        Partido* partido = crearCruceValido(p, s);

        if (!partido) break;

        partidosR16.agregar(partido, partidosR16.tamano());

        p.eliminar(partido->getEquipo1());
        s.eliminar(partido->getEquipo2());
    }

    Lista<Equipo*> s1;
    Lista<Equipo*> s2;

    for (int i = 0; i < s.tamano(); i++) {
        ITERACIONES++;
        s1.agregar(s.consultar(i), s1.tamano());
        s2.agregar(s.consultar(i), s2.tamano());
    }
    // S vs S
    while (s1.tamano() > 0 && s2.tamano() > 0) {
        ITERACIONES++;

        Partido* partido = crearCruceValido(s1, s2);

        if (partido != nullptr) {

            partidosR16.agregar(partido, partidosR16.tamano());

            s1.eliminar(partido->getEquipo1());
            s2.eliminar(partido->getEquipo2());

        } else {
            break;
        }
    }
}
void Mundial::simularR16() {

    for (int i = 0; i < partidosR16.tamano(); i++) {
        ITERACIONES++;

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

    for (int i = 0; i < equipos.tamano(); i++) {
        ITERACIONES++;

        Equipo* e = equipos.consultar(i);

        memoria += sizeof(Equipo);

        // jugadores
        memoria += e->getCantidadJugadores() * sizeof(Jugador);
    }

    for (int i = 0; i < partidos.tamano(); i++) {
        ITERACIONES++;

        memoria += sizeof(Partido);

        // arrays internos (11 jugadores por equipo)
        memoria += 22 * sizeof(StatsJugadorPartido);
    }

    for (int i = 0; i < partidosR16.tamano(); i++) {
        ITERACIONES++;

        memoria += sizeof(Partido);
        memoria += 22 * sizeof(StatsJugadorPartido);
    }

    memoria += equipos.tamano() * sizeof(Equipo*);
    memoria += partidos.tamano() * sizeof(Partido*);
    memoria += partidosR16.tamano() * sizeof(Partido*);

    memoria += sizeof(int) * 10;
    memoria += sizeof(double) * 5;

    cout << "\n Memoria estimada TOTAL: "
         << memoria << " bytes\n";
}
void Mundial::mostrarR16() {

    cout << "\n===== DIECISEISAVOS DE FINAL =====\n" << endl;

    cout << "Equipo A\tGoles\tEquipo B\tGanador\n";

    for (int i = 0; i < partidosR16.tamano(); i++) {
        ITERACIONES++;

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
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    // DIECISEISAVOS (R16)
    cout << "\n===== DIECISEISAVOS =====" << endl;
    generarR16();
    Lista<Equipo*> clasificados8;
    simularFase(partidosR16, equiposR8);
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    // OCTAVOS DE FINAL
    cout << "\n===== OCTAVOS DE FINAL =====" << endl;
    Lista<Partido*> octavos = generarFase(equiposR8, R16);
    Lista<Equipo*> clasificados4;
    simularFase(octavos, equiposR4);
    liberarPartidos(octavos);
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    // CUARTOS DE FINAL
    cout << "\n===== CUARTOS DE FINAL =====" << endl;
    Lista<Partido*> cuartos = generarFase(equiposR4, R16);
    Lista<Equipo*> clasificados2;
    simularFase(cuartos, clasificados2);
    liberarPartidos(cuartos);
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    // SEMIFINALES
    cout << "\n===== SEMIFINALES =====" << endl;
    Lista<Partido*> semifinal = generarFase(clasificados2, R16);
    Lista<Equipo*> finalistas;
    simularFase(semifinal, finalistas);
    liberarPartidos(semifinal);
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    //FINAL
    cout << "\n===== FINAL =====" << endl;
    Lista<Partido*> final = generarFase(finalistas, R16);
    Lista<Equipo*> campeonLista;
    simularFase(final, campeonLista);
    liberarPartidos(final);
    campeon = campeonLista.consultar(0);
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    // CAMPEÓN
    cout << "\n===== CAMPEÓN =====" << endl;
    cout << campeon->getPais() << endl;
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    // ESTADÍSTICAS FINALES
    cout << "\n===== ESTADISTICAS DEL MUNDIAL =====" << endl;
    mostrarEstadisticasFinales();
    cout << "\nIteraciones totales: " << ITERACIONES << endl;
    ITERACIONES=0;
    guardarJugadoresCSV();
    cout << "\nIteraciones totales: " << ITERACIONES << endl;

    mostrarMemoria();
}
Mundial::~Mundial() {
    for (int i = 0; i < partidosR16.tamano(); i++) {
        delete partidosR16.consultar(i);
    }
    for (int i = 0; i < partidos.tamano(); i++) {
        delete partidos.consultar(i);
    }
    delete[] equiposControl;
    delete[] ultimoDia;
}
void Mundial::registrarEnfrentamiento(Equipo* a, Equipo* b) {

    Enfrentamiento e;
    e.a = a;
    e.b = b;

    historial.agregar(e, historial.tamano());
}
bool Mundial::yaSeEnfrentaron(Equipo* a, Equipo* b) {

    for (int i = 0; i < historial.tamano(); i++) {
        ITERACIONES++;

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
        ITERACIONES++;

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
        ITERACIONES++;

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

    Equipo* top1 = nullptr;
    Equipo* top2 = nullptr;
    Equipo* top3 = nullptr;
    Equipo* top4 = nullptr;

    for (int i = 0; i < equipos.tamano(); i++) {
        ITERACIONES++;

        Equipo* e = equipos.consultar(i);

        if (!top1 || *e > *top1) {
            top4 = top3;
            top3 = top2;
            top2 = top1;
            top1 = e;
        }
        else if (!top2 || *e > *top2) {
            top4 = top3;
            top3 = top2;
            top2 = e;
        }
        else if (!top3 || *e > *top3) {
            top4 = top3;
            top3 = e;
        }
        else if (!top4 || *e > *top4) {
            top4 = e;
        }
    }

    cout << "\n TOP 4 DEL MUNDIAL:\n";

    if (top1) cout << "1. " << top1->getPais() << " (" << top1->getGolesFavor() << " goles)\n";
    if (top2) cout << "2. " << top2->getPais() << " (" << top2->getGolesFavor() << " goles)\n";
    if (top3) cout << "3. " << top3->getPais() << " (" << top3->getGolesFavor() << " goles)\n";
    if (top4) cout << "4. " << top4->getPais() << " (" << top4->getGolesFavor() << " goles)\n";
}


void Mundial::top3Goleadores() {

    Jugador* top1 = nullptr;
    Jugador* top2 = nullptr;
    Jugador* top3 = nullptr;

    for (int i = 0; i < equipos.tamano(); i++) {
        ITERACIONES++;

        Equipo* e = equipos.consultar(i);

        for (int j = 0; j < e->getCantidadJugadores(); j++) {
            ITERACIONES++;

            Jugador* actual = e->getJugador(j);

            // TOP 1
            if (top1 == nullptr || actual->getGoles() > top1->getGoles()) {
                top3 = top2;
                top2 = top1;
                top1 = actual;
            }
            // TOP 2
            else if (top2 == nullptr || actual->getGoles() > top2->getGoles()) {
                top3 = top2;
                top2 = actual;
            }
            // TOP 3
            else if (top3 == nullptr || actual->getGoles() > top3->getGoles()) {
                top3 = actual;
            }
        }
    }

    cout << "\n TOP 3 GOLEADORES DEL MUNDIAL:\n";

    if (top1) cout << "1. " << *top1 << endl;
    if (top2) cout << "2. " << *top2 << endl;
    if (top3) cout << "3. " << *top3 << endl;
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
        ITERACIONES++;

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
        ITERACIONES++;

        Equipo* e = equipos.consultar(i);

        if (e->getGolesFavor() > maxE) {
            maxE = e->getGolesFavor();
            mejorEquipo = e;
        }
    }

    cout << "\n EQUIPO MAS GOLEADOR:\n";
    cout << mejorEquipo->getPais()
         << " (" << maxE << " goles)\n";
    cout << "\n===== CONFEDERACIONES DOMINANTES =====\n";

    mostrarConfederacionDominante(clasificadosR16, "R16");
    mostrarConfederacionDominante(equiposR8, "R8");
    mostrarConfederacionDominante(equiposR4, "R4");


    // MEMORIA

    cout << "\n METRICAS:\n";

}
void Mundial::guardarJugadoresCSV() {

    ofstream archivo("jugadores.csv");

    archivo << "equipo;numero;nombre;apellido;goles;amarillas;rojas;faltas;minutos;asistencias;partidos\n";

    for (int i = 0; i < equipos.tamano(); i++) {
        ITERACIONES++;

        Equipo* eq = equipos.consultar(i);

        for (int j = 0; j < eq->getCantidadJugadores(); j++) {
            ITERACIONES++;

            Jugador* ju = eq->getJugador(j);

            archivo << eq->getPais() << ";"<< ju->getNumero() << ";"<< ju->getNombre() << ";"<< ju->getApellido() << ";"<< ju->getGoles() << ";"<< ju->getAmarillas() << ";"
                    << ju->getRojas() << ";"<< ju->getFaltas() << ";"<< ju->getMinutos() << ";"<< ju->getAsistencias() << ";"<< ju->getPartidos()<< "\n";
        }
    }

    archivo.close();

    cout << "\nArchivo jugadores.csv generado correctamente\n";
}
void Mundial::mostrarConfederacionDominante(Lista<Equipo*>& lista, string fase) {

    Lista<string> confs;
    Lista<int> conteos;

    for (int i = 0; i < lista.tamano(); i++) {
        ITERACIONES++;

        Equipo* e = lista.consultar(i);
        string conf = e->getConfederacion();

        bool encontrada = false;

        for (int j = 0; j < confs.tamano(); j++) {
            ITERACIONES++;

            if (confs.consultar(j) == conf) {
                int val = conteos.consultar(j);
                conteos.reemplazar(val + 1, j);
                encontrada = true;
                break;
            }
        }

        if (!encontrada) {
            confs.agregar(conf, confs.tamano());
            conteos.agregar(1, conteos.tamano());
        }
    }

    int max = 0;
    string mejor = "";

    for (int i = 0; i < conteos.tamano(); i++) {
        ITERACIONES++;

        if (conteos.consultar(i) > max) {
            max = conteos.consultar(i);
            mejor = confs.consultar(i);
        }
    }
    cout << "\nConfederacion dominante en " << fase << ": "
         << mejor << " (" << max << " equipos)\n";
}
