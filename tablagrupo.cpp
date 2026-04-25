#include "tablagrupo.h"

TablaGrupo::TablaGrupo() {}

void TablaGrupo::inicializar() {

    for (int i = 0; i < 4; i++) {

        StatsEquipo* s = new StatsEquipo;

        s->equipo = grupo->getEquipo(i);
        s->puntos = 0;
        s->gf = 0;
        s->gc = 0;

        tabla.agregar(s, tabla.tamano());
    }
}
void TablaGrupo::actualizar(Partido* p) {

    Equipo* A = p->getEquipo1();
    Equipo* B = p->getEquipo2();

    int gA = p->getGoles1();
    int gB = p->getGoles2();

    for (int i = 0; i < tabla.tamano(); i++) {

        StatsEquipo* s = tabla.consultar(i);

        if (s->equipo == A) {

            s->gf += gA;
            s->gc += gB;

            if (gA > gB) s->puntos += 3;
            else if (gA == gB) s->puntos += 1;
        }

        if (s->equipo == B) {

            s->gf += gB;
            s->gc += gA;

            if (gB > gA) s->puntos += 3;
            else if (gA == gB) s->puntos += 1;
        }
    }
}
void TablaGrupo::ordenar() {

    int n = tabla.tamano();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            StatsEquipo* a = tabla.consultar(j);
            StatsEquipo* b = tabla.consultar(j + 1);

            bool swap = false;

            if (a->puntos < b->puntos) swap = true;
            else if (a->puntos == b->puntos &&
                     (a->gf - a->gc) < (b->gf - b->gc)) swap = true;

            if (swap) {
                tabla.reemplazar(b, j);
                tabla.reemplazar(a, j + 1);
            }
        }
    }
}
TablaGrupo::TablaGrupo(Grupo* g) {
    this->grupo = g;
}

void TablaGrupo::mostrar() {
    ordenar();

    cout << "==============================\n";
    cout << "      GRUPO " << grupo->getLetra() << "\n";
    cout << "==============================\n";

    cout << "Equipo        PJ  GF  GC  Pts  DG\n";
    cout << "----------------------------------\n";

    for (int i = 0; i < tabla.tamano(); i++) {

        StatsEquipo* s = tabla.consultar(i);

        int dg = s->gf - s->gc;

        cout << s->equipo->getPais();

        // alineación simple (ajuste de espacios)
        int espacios = 14 - s->equipo->getPais().length();
        for (int j = 0; j < espacios; j++) cout << " ";

        cout << 3 << "   "   // PJ fijo en grupos
             << s->gf << "   "
             << s->gc << "   "
             << s->puntos << "   "
             << dg << endl;
    }

    cout << "----------------------------------\n\n";
}
StatsEquipo* TablaGrupo::get(int i) {
    return tabla.consultar(i);
}
Lista<Equipo*> TablaGrupo::clasificados() {

    Lista<Equipo*> res;

    ordenar();

    for (int i = 0; i < 2; i++) {
        res.agregar(tabla.consultar(i)->equipo, res.tamano());
    }

    return res;
}
/*TablaGrupo::~TablaGrupo() {
    for (int i = 0; i < tabla.tamano(); i++) {
        delete tabla.consultar(i);
    }
}*/
